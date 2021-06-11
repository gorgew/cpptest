#define STB_IMAGE_IMPLEMENTATION
#include <Injector.hpp>
#include <Window.hpp>
#include <PhysicsComponents.hpp>
#include <GraphicsComponents.hpp>
#include <GraphicsSystem.hpp>
#include <FontBuilder.hpp>
#include <fmt/core.h>

#include <State.hpp>
#include <StartState.hpp>

#include <MouseEventSystem.hpp>

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
    FontBuilder f_builder{injector};
    f_builder.add_font("arial", "../resources/FantasqueSansMono-Regular.ttf", 96);

    entt::registry registry;
    f_builder.add_string(registry, "hello world", "arial", 96, glm::vec3(0, 800.0f, 0.0f), glm::vec3(1.0));

    std::shared_ptr<State> game_state = std::make_shared<StartState>(injector, registry);
    
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
            game_state->handle_event(registry, event);
            
            if (event.type == SDL_QUIT) {
                break;
            } else if (event.type == SDL_KEYDOWN) {
                
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                } else if (event.key.repeat != 0) {
                    SDL_FlushEvent(SDL_KEYDOWN);
                }
                
            } else if (event.type == SDL_WINDOWEVENT) {
                
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    g_system.resize(event.window.data1, event.window.data2);
                }
            }
        }
        game_state->process_systems(registry);
        g_system.draw(registry);
        SDL_GL_SwapWindow(window.windowPtr);
        injector->audio.handle_request(registry);
        //Check if going to next state
        if (game_state->ready_next()) {

            game_state = game_state->next(registry);
        }
        //Sleep until next frame
        frame_clock = high_resolution_clock::now();
        sleep_secs = 1.0 / 12 - (frame_clock - next_clock).count() / 1e9;
        if (sleep_secs > 0) {
            std::this_thread::sleep_for(nanoseconds((int64_t)(sleep_secs * 1e9)));
        }
        prev_clock = next_clock;
    }

    g_system.free_frame_lists(registry);

    return 0;
}