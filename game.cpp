#define STB_IMAGE_IMPLEMENTATION
#include <Injector.hpp>
#include <Window.hpp>
#include <PhysicsComponents.hpp>
#include <GraphicsComponents.hpp>
#include <GraphicsSystem.hpp>
#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <chrono>
#include <memory>

using namespace std::chrono;

int main(void) {

    auto injector = std::make_shared<Injector>();
    fmt::print("Game initializing\n");
    Window window {"Game", injector->config.height, injector->config.width};
    GraphicsSystem g_system{injector};

    entt::registry registry;

    //TESTING GRAPHICS SYSTEM!!
    //REMOVE LATER!!

    //MAKING THE OPENGL DATA
    injector->tex_man.add_2d_array_texture("blank", "../resources/NumsPacked.png", 32, 32, 6);
    injector->shader_man.add_shader("sprites-v", "../resources/sprites.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("tex", "../resources/array-tex.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("sprites", {"sprites-v", "tex"});
    injector->shader_man.use("sprites");

    int id = injector->shader_man.get_program_id("sprites");
    glUniform1i(glGetUniformLocation(id, "iTexture"), 0);
    fmt::print("texture_id: {}\n", injector->tex_man.get_id("blank"));
    fmt::print("shader id: {}\n", id);

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //CREATING THE ENTITY

    struct frame my_static_graphic = gorge::build_frame(injector, 1.0f, 1.0f, 
            "blank", 2, "sprites");
    struct frame_node my_animated_graphic = gorge::build_frame_node(injector, 1.0f, 1.0f, 
            "blank", 0, 5, "sprites");
    const auto entity = registry.create();
    //registry.emplace<static_graphic>(entity, my_static_graphic);
    registry.emplace<frame_node>(entity, my_animated_graphic);
    registry.emplace<position>(entity, glm::vec3(0.0f));

    //Timing clocks
    auto prev_clock = high_resolution_clock::now();
    auto next_clock = high_resolution_clock::now();
    auto frame_clock = high_resolution_clock::now();
    float delta_time = 0.0f;
    float sleep_secs = 0.0f;

    SDL_Event event;
    while (true) {
        
        next_clock = high_resolution_clock::now();
        delta_time = (next_clock - prev_clock).count() / 1e9;

        fmt::print("Frame time: {} ms\n", delta_time * 1e3);

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                break;

            } else if (event.type == SDL_KEYUP && 
                    event.key.keysym.sym == SDLK_ESCAPE) {
                break;

            } else if (event.type == SDL_WINDOWEVENT) {
            }
        }
        
        g_system.draw(registry);
        SDL_GL_SwapWindow(window.windowPtr);
        //Sleep until next frame
        frame_clock = high_resolution_clock::now();
        sleep_secs = 1.0 / 12 - (frame_clock - next_clock).count() / 1e9;
        if (sleep_secs > 0) {
            std::this_thread::sleep_for(nanoseconds((int64_t)(sleep_secs * 1e9)));
        }
        prev_clock = next_clock;
    }
    return 0;
}