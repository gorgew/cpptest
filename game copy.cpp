#define STB_IMAGE_IMPLEMENTATION
#include <Injector.hpp>
#include <Window.hpp>
#include <PhysicsComponents.hpp>
#include <GraphicsComponents.hpp>
#include <GraphicsSystem.hpp>
#include <FontBuilder.hpp>
#include <fmt/core.h>

#include <MouseEventSystem.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <chrono>
#include <memory>

using namespace std::chrono;

struct empty_struct {

};

int main(void) {

    //entt::entity entity_test = entt::null;
    //fmt::print("null {}\n", static_cast<std::underlying_type_t<entt::entity>>(entity_test));
    auto injector = std::make_shared<Injector>();

    fmt::print("Game initializing\n");
    Window window {"Game", injector->config.height, injector->config.width};
    GraphicsSystem g_system{injector};
    FontBuilder f_builder{injector};
    f_builder.add_font("arial", "../resources/FantasqueSansMono-Regular.ttf", 96);

    auto my_char_data = f_builder.get_char("arial", 96, 'm');
    fmt::print("my_char_data info:\n tex_id: {}, bearing_x {}\n", my_char_data.tex_id, my_char_data.bearing_x);

    entt::registry registry;
    f_builder.add_string(registry, "hello world", "arial", 96, glm::vec3(0, 800.0f, 0.0f), glm::vec3(1.0));
    //TESTING GRAPHICS SYSTEM!!
    //REMOVE LATER!!
    //MAKING THE OPENGL DATA
    injector->tex_man.add_2d_array_texture("blank", "../resources/NumsPacked.png", 32, 32, 6);
    injector->shader_man.add_shader("sprites-v", "../resources/sprites.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("array-tex", "../resources/array-tex.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("sprites", {"sprites-v", "array-tex"});
    injector->shader_man.use("sprites");

    int id = injector->shader_man.get_program_id("sprites");
    glUniform1i(glGetUniformLocation(id, "iTexture"), 0);
    fmt::print("texture_id: {}\n", injector->tex_man.get_id("blank"));
    fmt::print("shader id: {}\n", id);

    //glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(injector->config.width), 0.0f, static_cast<float>(injector->config.height));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //CREATING THE ENTITY
   
    struct array_frame_node my_animated_graphic = gorge::build_array_frame_node(injector, 200.0f, 200.0f, 
            "blank", 0, 5, "sprites");
    const auto entity = registry.create();
    //registry.emplace<static_graphic>(entity, my_static_graphic);
    registry.emplace<array_frame_node>(entity, my_animated_graphic);
    registry.emplace<position>(entity, glm::vec3(200.0f, 200.0f, 0.0f));
    //registry.emplace<position>(entity, glm::vec3(1.0f));

    struct frame my_char_frame = gorge::build_frame_flipped(injector, 1.0f, 1.0f, "blank", "fonts");
    my_char_frame.tex_id  = my_char_data.tex_id;
    //my_char_frame.tex_array_id = 70;
    fmt::print("char texid {}\n", my_char_frame.tex_id);
    const auto entity1 = registry.create();
    registry.emplace<frame>(entity1, my_char_frame);
    registry.emplace<position>(entity1, glm::vec3(400.0f, 400.0f, 0.0f));
    //registry.emplace<position>(entity1, glm::vec3(1.0f, 1.0f, 0.0f));
    
    std::function<void(entt::registry&)> check_a = [](entt::registry& registry){
        fmt::print("HELLO WORLD\n");
        auto view = registry.view<position>();
        for (auto [entity, pos] : view.each()) {
            pos.pos = pos.pos + glm::vec3(0.0f, 2.0f, 0.0f);
        }
    };
    injector->key_event_system.add_keydown_handler(SDLK_a, check_a);

    MouseEventSystem mouse_sys = {injector};
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

            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                } else if (event.key.repeat != 0) {
                    SDL_FlushEvent(SDL_KEYDOWN);
                }
                injector->key_event_system.handle_event(registry, event);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {

                mouse_sys.handle_event(registry, event);
    
            } else if (event.type == SDL_WINDOWEVENT) {
                
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    g_system.resize(event.window.data1, event.window.data2);
                }
            }
        }
        injector->key_event_system.execute_holds(registry);
        g_system.draw(registry);
        SDL_GL_SwapWindow(window.windowPtr);

        //Sleep until next frame
        frame_clock = high_resolution_clock::now();
        sleep_secs = 1.0 / 24 - (frame_clock - next_clock).count() / 1e9;
        if (sleep_secs > 0) {
            std::this_thread::sleep_for(nanoseconds((int64_t)(sleep_secs * 1e9)));
        }
        prev_clock = next_clock;
    }

    g_system.free_frame_lists(registry);

    return 0;
}