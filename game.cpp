#define STB_IMAGE_IMPLEMENTATION
#include <Injector.hpp>
#include <Window.hpp>
#include <PhysicsComponents.hpp>
#include <GraphicsComponents.hpp>
#include <GraphicsSystem.hpp>
#include <FontBuilder.hpp>
#include <ScriptEngine.hpp>
#include <ResourceManager.hpp>
#include <fmt/core.h>

#include <State.hpp>
#include <StartState.hpp>

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

    std::shared_ptr<ScriptEngine> scripts = std::make_shared<ScriptEngine>();

    std::shared_ptr<ResourceManager> resources = std::make_shared<ResourceManager>();
    resources->load_character_resources(scripts->lua, injector);
    fmt::print("Resources Loaded\n");

    std::shared_ptr<State> game_state; 
    game_state = std::make_shared<StartState>(injector, registry, scripts, resources);

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

    /*
    injector->tex_man.add_texture("profile_pic", "../resources/bush.png", false);
    struct nk_image profile_pic = nk_image_id(static_cast<int>(injector->tex_man.get_id("profile_pic")));
    */

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

        while (SDL_PollEvent(&event)) {

            game_state->handle_event(registry, event);
            ImGui_ImplSDL2_ProcessEvent(&event);
            nk_sdl_handle_event(&event);

            if (event.type == SDL_QUIT) {
                goto EXIT;
            } else if (event.type == SDL_KEYDOWN) {
                
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    goto EXIT;
                } else if (event.key.repeat != 0) {
                    SDL_FlushEvent(SDL_KEYDOWN);
                }
                
            } else if (event.type == SDL_WINDOWEVENT) {
                
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    g_system.resize(event.window.data1, event.window.data2);
                    game_state->resize(event.window.data1, event.window.data2);
                }
            } else if (event.type == SDL_JOYBUTTONDOWN) {
                goto EXIT;
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

        game_state->display_ui(ctx);
        
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

EXIT:
    fmt::print("exit\n");
    g_system.free_frame_lists(registry);
    
    nk_sdl_shutdown();
    
    return 0;
}