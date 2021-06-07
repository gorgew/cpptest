#include "GraphicsSystem.hpp"
#include "GraphicsComponents.hpp"
#include "FontComponents.hpp"
#include <PhysicsComponents.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <fmt/core.h>

GraphicsSystem::GraphicsSystem(std::shared_ptr<Injector> injector) {

    this->injector = injector;
    fbo_width = injector->config.width;
    fbo_height = injector->config.height;
    default_width = fbo_width;
    default_height = fbo_height;
    //Load glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fmt::print("OpenGL loading failed");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    offscreenBuffer.reset(new FrameBufferObject());
    offscreenBuffer->addTexture2D(fbo_width, fbo_height);
    offscreenBuffer->addDepthStencilRenderBuffer(fbo_width, fbo_height);
    offscreenBuffer->checkCompiled();
    
    injector->shader_man.add_shader("v_tex", "../resources/vert_tex.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("tex", "../resources/tex.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("v_tex/tex", {"v_tex", "tex"});
    
    double_buffer_program = injector->shader_man.get_program_id("v_tex/tex");
    
    injector->vert_man.add_rect("buffer_rect", 2.0f, 2.0f);

    buffer_vertices = injector->vert_man.get_array_id("buffer_rect");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, injector->config.width, injector->config.height);
}

void GraphicsSystem::set_program(struct frame f) {
    if (active_program != f.program_id) {
        active_program = f.program_id;
        glUseProgram(active_program);
    }
}

void GraphicsSystem::draw_frame(struct frame f) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, f.tex_id);
    glBindVertexArray(f.vert_arr_id);
    glDrawArrays(f.primitive_type, 0, f.vert_count);
}

void GraphicsSystem::draw_array_frame(struct frame f) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, f.tex_id);
    glBindVertexArray(f.vert_arr_id);
    glDrawArrays(f.primitive_type, 0, f.vert_count);
}

void GraphicsSystem::draw_frame_component(struct frame f, struct position pos) {

        set_program(f);
        model = glm::translate(glm::mat4(1.0f), pos.pos);
        glUniformMatrix4fv(glGetUniformLocation(active_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        draw_frame(f);      
}

void GraphicsSystem::draw_char_component(struct char_frame_data f, struct position pos) {

        set_program(f);
        
        model = glm::translate(glm::mat4(1.0f), pos.pos);
        glUniformMatrix4fv(glGetUniformLocation(active_program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glUniform2f(glGetUniformLocation(active_program, "size"), f.width, f.height);
        glUniform3fv(glGetUniformLocation(active_program, "color"), 1, glm::value_ptr(f.color));

        draw_frame(f);
}

void GraphicsSystem::draw_array_frame_component(struct array_frame f, struct position pos) {

        set_program(f);
        fmt::print("tex_id: {}\n", f.tex_id);
        model = glm::translate(glm::mat4(1.0f), pos.pos);
        glUniformMatrix4fv(glGetUniformLocation(active_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(active_program, "layer"), f.tex_layer);  
        draw_array_frame(f);      
}

void GraphicsSystem::draw(entt::registry& registry) {

    //draw to the buffer
    
    offscreenBuffer->activate();
    glViewport(0, 0, fbo_width, fbo_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    auto frame_view = registry.view<frame, position>();
    for (auto [entity, f, pos] : frame_view.each()) {
        draw_frame_component(f, pos);
    }

    auto array_frame_view = registry.view<array_frame, position>();
    for (auto [entity, f, pos] : array_frame_view.each()) {
        draw_array_frame_component(f, pos);
    }

    auto frame_list_view = registry.view<array_frame_node, position>();
    for (auto [entity, f_node, pos] : frame_list_view.each()) {
        draw_array_frame_component(f_node, pos);
        f_node = *(f_node.next);
    }

    auto char_frame_view = registry.view<char_frame_data, position>();
    for (auto [entity, char_f, pos] : char_frame_view.each()) {
        draw_char_component(char_f, pos);
    }
    //Draw to the main buffer
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, default_width, default_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(buffer_vertices);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offscreenBuffer->texID);
    active_program = double_buffer_program;
    glUseProgram(double_buffer_program);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void GraphicsSystem::free_frame_list(array_frame_node f_node) {

    auto min = f_node.tex_layer;

    //Find the min layer in the frame list
    array_frame_node* it = f_node.next;
    while (it->tex_layer != min) {
        if (it->tex_layer < min) {
            break;
        }
        it = it->next;
    }
    std::free(it);
}

void GraphicsSystem::free_frame_lists(entt::registry& registry) {
    auto view = registry.view<array_frame_node>();
    for (auto [entity, f_node] : view.each()) {
        free_frame_list(f_node);
    }
}

void GraphicsSystem::resize(int width, int height) {
    default_width = width;
    default_height = height;
}

GraphicsSystem::~GraphicsSystem() {

}
