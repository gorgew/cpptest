#include "GraphicsSystem.hpp"
#include "GraphicsComponents.hpp"

#include <PhysicsComponents.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <fmt/core.h>

GraphicsSystem::GraphicsSystem(std::shared_ptr<Injector> injector) {

    this->injector = injector;

    //Load glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fmt::print("OpenGL loading failed");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);

    /*
    offscreenBuffer.reset(new FrameBufferObject());
    offscreenBuffer->addTexture2D(injector->config.width, injector->config.height);
    offscreenBuffer->addDepthStencilRenderBuffer();
    offscreenBuffer->checkCompiled();
    
    injector->shader_man.add_shader("v_tex", "../resources/v_tex.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("tex", "../resources/tex.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("v_tex/tex", {"v_tex", "tex"});

    double_buffer_program = injector->shader_man.get_program_id("v_tex/tex");
    
    injector->vert_man.add_rect("buffer_rect", 2.0f, 2.0f);

    buffer_vertices = injector->vert_man.get_array_id("buffer_rect");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    offscreenBuffer->activate();
    */
   glViewport(0, 0, injector->config.width, injector->config.height);
}

void GraphicsSystem::draw_component(struct frame f, struct position pos) {

        if (active_program != f.program_id) {
            active_program = f.program_id;
            glUseProgram(active_program);
        }
        model = glm::translate(glm::mat4(1.0f), pos.pos);
        glUniformMatrix4fv(glGetUniformLocation(active_program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, f.tex_array_id);
        glUniform1f(glGetUniformLocation(active_program, "layer"), f.tex_layer);

        glBindVertexArray(f.vert_arr_id);
        glDrawArrays(f.primitive_type, 0, f.vert_count);
}

void GraphicsSystem::draw(entt::registry& registry) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view_of_static = registry.view<frame, position>();
    for (auto [entity, f, pos] : view_of_static.each()) {
        draw_component(f, pos);
    }

    auto view_of_animated = registry.view<frame_node, position>();
    for (auto [entity, animated_g, pos] : view_of_animated.each()) {
        draw_component(animated_g, pos);
        fmt::print("layer of next {}\n" , animated_g.next->tex_layer);
        animated_g = *(animated_g.next);
    }
}

GraphicsSystem::~GraphicsSystem() {

}

/*
void Graphics::draw() {
    
    offscreenBuffer->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    //sceneManagerPtr->drawScene();
    //std::cout<<"drawing\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(defaultBufferQuadArray->getVAOID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offscreenBuffer->texID);
    defaultBufferProgram->use();
    defaultBufferProgram->setUniform("iTexture", 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
}
*/
