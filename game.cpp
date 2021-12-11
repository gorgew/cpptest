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

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

using namespace std::chrono;
using namespace entt::literals;

int main(void) {

    auto injector = std::make_shared<Injector>();
    
    fmt::print("Game initializing\n");
    Window window {"Game", injector->config.height, injector->config.width};
    GraphicsSystem g_system{injector};
    g_system.frame_delay = 5;
    FontBuilder f_builder{injector};
    f_builder.add_font("arial", "../resources/FantasqueSansMono-Regular.ttf", 96);

    entt::registry registry;
    f_builder.add_string(registry, "hello world", "arial", 96, glm::vec3(0, 800.0f, 0.0f), glm::vec3(1.0));

    std::shared_ptr<State> game_state; 
    game_state = std::make_shared<StartState>(injector, registry);
    
    fmt::print("Scene constructed\n");
    
    //IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& imgui_io = ImGui::GetIO(); (void) imgui_io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window.windowPtr, window.glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Timing clocks
    auto prev_clock = high_resolution_clock::now();
    auto next_clock = high_resolution_clock::now();
    auto frame_clock = high_resolution_clock::now();
    float delta_time = 0.0f;
    float sleep_secs = 0.0f;

    bool show_demo = true;
    SDL_Event event;
    while (true) {
        
        next_clock = high_resolution_clock::now();
        delta_time = (next_clock - prev_clock).count() / 1e9;

        //fmt::print("Frame time: {} ms\n", delta_time * 1e3);

        if (SDL_PollEvent(&event)) {
            game_state->handle_event(registry, event);
            ImGui_ImplSDL2_ProcessEvent(&event);
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
                    game_state->resize(event.window.data1, event.window.data2);
                }
            }
        }
        game_state->process_systems(registry);
        
        g_system.draw(registry);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window.windowPtr);
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&show_demo);
        
        ImGui::Render();
        glViewport(0, 0, (int)imgui_io.DisplaySize.x, (int)imgui_io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(window.windowPtr);
        injector->audio.handle_request();
        //Check if going to next state
        if (game_state->ready_next()) {

            game_state = game_state->next(registry);
        }
        //Sleep until next frame
        frame_clock = high_resolution_clock::now();
        sleep_secs = 1.0 / 60 - (frame_clock - next_clock).count() / 1e9;
        if (sleep_secs > 0) {
            std::this_thread::sleep_for(nanoseconds((int64_t)(sleep_secs * 1e9)));
        }
        prev_clock = next_clock;

        
    }
    
    g_system.free_frame_lists(registry);
    
    
    return 0;
}