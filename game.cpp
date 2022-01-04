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

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_sdl_gl3.h>

using namespace std::chrono;
using namespace entt::literals;

constexpr float FRAME_TIME = 1.0f / 120.0f;

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

    //nuklear
    struct nk_context *ctx;

    ctx = nk_sdl_init(window.windowPtr);
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
    }

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
        game_state->delta_time = delta_time;
        //fmt::print("Frame time: {} ms\n", delta_time * 1e3);

        nk_input_begin(ctx);
        

        if (SDL_PollEvent(&event)) {

            game_state->handle_event(registry, event);
            ImGui_ImplSDL2_ProcessEvent(&event);
            nk_sdl_handle_event(&event);

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

        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                printf("button pressed!\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 22, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
        }
        nk_end(ctx);

        g_system.viewport();
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);;

        SDL_GL_SwapWindow(window.windowPtr);
        injector->audio.handle_request();
        //Check if going to next state
        if (game_state->ready_next()) {

            game_state = game_state->next(registry);
        }
        //Sleep until next frame
        frame_clock = high_resolution_clock::now();
        sleep_secs = FRAME_TIME - (frame_clock - next_clock).count() / 1e9;
        if (sleep_secs > 0) {
            std::this_thread::sleep_for(nanoseconds((int64_t)(sleep_secs * 1e9)));
        }
        prev_clock = next_clock;

        
    }
    
    g_system.free_frame_lists(registry);
    
    nk_sdl_shutdown();
    
    return 0;
}