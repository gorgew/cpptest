#include "GraphicsSystem.hpp"
#include "GraphicsComponents.hpp"
#include "FontComponents.hpp"
#include <GameObjectComponents.hpp>
#include <PhysicsComponents.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <fmt/core.h>

GraphicsSystem::GraphicsSystem() {
    fbo_width = locator.get_config()->width;
    fbo_height = locator.get_config()->height;
    default_width = fbo_width;
    default_height = fbo_height;
    //Load glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fmt::print("OpenGL loading failed");
        exit(1);
    }

    //glEnable(GL_DEPTH_TEST);
    
    offscreenBuffer.reset(new FrameBufferObject());
    offscreenBuffer->addTexture2D(fbo_width, fbo_height);
    offscreenBuffer->addDepthStencilRenderBuffer(fbo_width, fbo_height);
    offscreenBuffer->checkCompiled();
    
    locator.get_shaders()->add_shader("db_vert", "resources/vert_tex.vert", GL_VERTEX_SHADER);
    locator.get_shaders()->add_shader("db_tex", "resources/tex.frag", GL_FRAGMENT_SHADER);
    locator.get_shaders()->add_program("double_buffer", {"db_vert", "db_tex"});
    
    double_buffer_program = locator.get_shaders()->get_program_id("double_buffer");
    
    locator.get_vertices()->add_rect("buffer_rect", 2.0f, 2.0f);

    buffer_vertices = locator.get_vertices()->get_array_id("buffer_rect");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, locator.get_config()->width, locator.get_config()->height);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    glDrawArrays(GL_TRIANGLE_FAN, 0, f.vert_count);
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
        model = glm::translate(glm::mat4(1.0f), pos.pos);
        glUniformMatrix4fv(glGetUniformLocation(active_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(active_program, "layer"), f.tex_layer);  
        draw_array_frame(f);      
}

void GraphicsSystem::draw(entt::registry& registry, const float& delta_time) {

    //draw to the buffer
    
    offscreenBuffer->activate();
    glViewport(0, 0, fbo_width, fbo_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnablei(GL_BLEND, offscreenBuffer->fboID);
    glEnablei(GL_BLEND, 0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Get world depth right

    frame_count++;
    if (frame_count == frame_delay) {
        frame_count = 0;
    }

    auto t_view = registry.view<array_frame, position, terrain>(
        entt::exclude<environment, character>);
    for (auto [entity, f, pos, t] : t_view.each()) {
        draw_array_frame_component(f, pos);
    }

    auto e_view = registry.view<array_frame, position, environment>(
        entt::exclude<terrain, character>);
    for (auto [entity, f, pos, e] : e_view.each()) {
        draw_array_frame_component(f, pos);
    }

    auto c_view = registry.view<array_frame, position, character>(
        entt::exclude<animation>);
    for (auto [entity, f, pos, t] : c_view.each()) {
        draw_array_frame_component(f, pos);
    }

    //Very branchy
    auto animation_view = registry.view<animation, position>();
    for (auto [entity, a, pos] : animation_view.each()) {
        a.accumulator += static_cast<int>(delta_time * 1000 );
        struct position temp = {pos.pos + a.offset};
        draw_array_frame_component(a.frames[a.curr], temp);
        if (a.accumulator > a.timings[a.curr]) {
            //fmt::print("asdfasdf {}\n", a.frame_count);
            a.accumulator -= a.timings[a.curr];
            a.curr++;
            
            if (a.curr == a.frame_count) {
                if (!a.loop) {
                    registry.remove<animation>(entity);
                }
                a.curr = 0;
            }
        }
        
    }

    auto frame_view = registry.view<frame, position>(
        entt::exclude<terrain, environment, character>);
    for (auto [entity, f, pos] : frame_view.each()) {
        draw_frame_component(f, pos);
    }

    auto array_frame_view = registry.view<array_frame, position>(
        entt::exclude<terrain, environment, character>);
    for (auto [entity, f, pos] : array_frame_view.each()) {
        draw_array_frame_component(f, pos);
    }

    //Fonts
    auto char_frame_view = registry.view<char_frame_data, position>(
        entt::exclude<terrain, environment, character>);
    for (auto [entity, char_f, pos] : char_frame_view.each()) {
        draw_char_component(char_f, pos);
    }
    //Draw to the main buffer
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisablei(GL_BLEND, 0);
    glViewport(0, 0, default_width, default_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(buffer_vertices);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offscreenBuffer->texID);
    active_program = double_buffer_program;
    glUseProgram(double_buffer_program);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void GraphicsSystem::resize(int width, int height) {
    default_width = width;
    default_height = height;
}

void GraphicsSystem::viewport() {
    glViewport(0, 0, locator.get_config()->width, locator.get_config()->height);
}

GraphicsSystem::~GraphicsSystem() {

}
