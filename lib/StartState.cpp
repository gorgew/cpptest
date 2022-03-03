#include "StartState.hpp"

#include <fmt/format.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GraphicsComponents.hpp>
#include <PhysicsComponents.hpp>
#include "TileMap.hpp"
#include "WorldCacheSystem.hpp"
#include <color-vec.hpp>
#include "UboStructs.hpp"
#include "CreditsState.hpp"

void StartState::build_key_handlers() {

   std::function<void(entt::registry&)> pan_right = [&](entt::registry& registry) mutable {
        camera->pan(1, 0, 0, delta_time);
    };

    std::function<void(entt::registry&)> pan_left = [=, this](entt::registry& registry) {
        camera->pan(-1, 0, 0, delta_time);
    };
    std::function<void(entt::registry&)> pan_up = [=, this](entt::registry& registry) {
        camera->pan(0, 1, 0, delta_time);
    };
    std::function<void(entt::registry&)> pan_down = [=, this](entt::registry& registry) {
        camera->pan(0, -1, 0, delta_time);
    };
    std::function<void(entt::registry&)> placeholder = [](entt::registry&){

    };
    key_system.add_held_key_handler(SDLK_d, pan_right, placeholder);
    key_system.add_held_key_handler(SDLK_a, pan_left, placeholder);
    key_system.add_held_key_handler(SDLK_w, pan_up, placeholder);
    key_system.add_held_key_handler(SDLK_s, pan_down, placeholder);
    
    std::function<void(entt::registry&)> pitch_left = [=, this](entt::registry& registry) {
        camera->update_pitch(-1.0f);
    };
    std::function<void(entt::registry&)> pitch_right = [=, this](entt::registry& registry) {
        camera->update_pitch(1.0f);
    };
    std::function<void(entt::registry&)> yaw_down = [=, this](entt::registry& registry) {
        camera->update_yaw(1.0f);
    };
    std::function<void(entt::registry&)> yaw_up = [=, this](entt::registry& registry) {
        camera->update_yaw(-1.0f);
    };
    key_system.add_held_key_handler(SDLK_q, pitch_left, placeholder);
    key_system.add_held_key_handler(SDLK_e, pitch_right, placeholder);
    key_system.add_held_key_handler(SDLK_z, yaw_up, placeholder);
    key_system.add_held_key_handler(SDLK_x, yaw_down, placeholder);
}

void StartState::build_mouse_handlers() {

    std::function<void(entt::registry&)> zoom_in = [=, this](entt::registry& registry) {
        camera->zoom(1, delta_time);
    };
    std::function<void(entt::registry&)> zoom_out = [=, this](entt::registry& registry) {
        camera->zoom(-1, delta_time);
    };
    
    mouse_system.add_wheel_handler(true, zoom_in);
    mouse_system.add_wheel_handler(false, zoom_out);

    std::function<void(entt::registry&)> move_cursor = [=, this](entt::registry& registry) {
        tmap.move_cusor(registry, mouse_system.world_x, mouse_system.world_y);
    };

    mouse_system.add_mousedown_handler(move_cursor, SDL_BUTTON_LEFT);
    mouse_system.set_motion_handler(move_cursor);
}

void StartState::build_music() {
    /*
    injector->audio.add_music("dd-town", "resources/dd-town.wav");
    injector->audio.enqueue_music("dd-town");
    //registry.emplace<audio_request>(music_command, "dd-town");

    injector->audio.add_effect("slow-killer", "resources/slow_killer.wav");
    
    std::function<void(entt::registry&)> play_on_y= [=, this](entt::registry& registry) {
        injector->audio.enqueue_effect("slow-killer");
    };
    key_system.add_keydown_handler(SDLK_y, play_on_y);
    */
}

void StartState::build_gfx() {
    fmt::print("Building gfx\n");
    //Other systems
    injector->tex_man.add_2d_array_texture("blank", "resources/NumsPacked.png", 32, 32, 6);
    injector->shader_man.add_shader("world.vert", "resources/world.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("world.frag", "resources/world.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("world", {"world.vert", "world.frag"});
    injector->shader_man.use("world");
    program_id  = injector->shader_man.get_program_id("world");

    //sglUniform1i(glGetUniformLocation(program_id, "iTexture"), 0);
    

    injector->shader_man.add_shader("billboard.vert", "resources/billboard.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_program("billboard", {"billboard.vert", "world.frag"});

    //glUniform1i(glGetUniformLocation(injector->shader_man.get_program_id("billboard"), "iTexture"), 0);

    injector->shader_man.add_ubo("camera_ubo", sizeof(camera_data), 0);
    injector->shader_man.bind_ubo("world", "camera_ubo", 0);

    camera.reset(new Camera(static_cast<float>(injector->config.width), 
        static_cast<float>(injector->config.height),
        100.0f, 
        1.0f,
        injector->shader_man.get_ubo("camera_ubo")));


    //auto light_color = color_vec::make_rgb(255, 255, 100, 1.0f);
    //glUniform3fv(glGetUniformLocation(program_id, "light_color"), 1, light_color);

    /*
    injector->tex_man.add_2d_array_texture("dummy-menu", "resources/dummy-menu.png", 128, 128, 2);
    injector->tex_man.add_2d_array_texture("tiles", "resources/iso-h-v1.png", 16, 16, 4);
    */
};
void StartState::build_scene(entt::registry& registry) {
    fmt::print("Building Scene...\n");
   /*
    struct array_frame_node my_animated_graphic = gorge::build_array_frame_node(injector, 200.0f, 200.0f, 
            "blank", 0, 5, "sprites");
    const auto entity = registry.create();
    registry.emplace<array_frame_node>(entity, my_animated_graphic);
    registry.emplace<position>(entity, glm::vec3(200.0f, 200.0f, 0.0f));

    struct array_frame my_tile = gorge::build_array_frame(injector, 100.0f, 100.0f, "tiles", 1, "sprites");
    const auto tile = registry.create();
    registry.emplace<array_frame>(tile, my_tile);
    registry.emplace<position>(tile, glm::vec3(400.0f, 200.0f, 0.0f));
    */ 
   {
        std::vector<int> frames = {0, 1, 2, 3};
        std::vector<int> timings = {1000, 1000, 1000, 1000};
        resources->add_animation("anim", "resources/anim.png", 16, 16, 4, 100, 100, "billboard",
            true, frames, timings);
   }
  
    injector->tex_man.add_2d_array_texture("art", "resources/programmer-art.png", 16, 16, 12);
    
    tmap = {injector, "art", "world", "billboard", registry};
    tmap.load_tileset(scripts->lua);
    tmap.load_tiles(scripts->lua);
    tmap.load_map("Test", scripts->lua, registry);

    std::function<void(entt::registry&)> shift_down = [](entt::registry& registry) {
    
        fmt::print("surprise\n");
    };
    /*
    const auto button = registry.create();
    registry.emplace<position>(button, glm::vec3(400.0f, 400.0f, 0.0f));
    registry.emplace<rect_button>(button, 800.0f, 800.0f, shift_down);
    */
};

void StartState::process_systems(entt::registry& registry) {
    if (systems_enabled) {
        key_system.execute_holds(registry);

        
        if (tmap.get_char_on_cursor(registry) != entt::null) {
            ui_show_character_hover = true;
            update_char_hover_data(registry);
        }
        else {
            ui_show_character_hover = false;
        }
        
        
    }
}

std::shared_ptr<State> StartState::next(entt::registry& registry) {

    registry.clear();
    return std::make_shared<CreditsState>(injector, registry);
}

void StartState::resize(int x, int y) {
    camera->resize(x, y);
    injector->config.width = x;
    injector->config.height = y;
}

void StartState::link_scripts() {
    
}

void StartState::display_ui(nk_context* ctx) {

    if (ui_show_demo) {
        display_demo(ctx);
    }

    if (ui_show_character_hover) {
        show_character_hover_ui(ctx);
    }

    
}

void StartState::display_demo(nk_context* ctx) {
        
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
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
            
            nk_layout_row_static(ctx, 50, 50, 1);
            //nk_image(ctx, profile_pic);
            
        }
}

void StartState::update_char_hover_data(entt::registry& registry) {
    
    std::string char_name = tmap.last_char;
    
    if (char_name != char_hover_data.char_name) {

        resources->set_animation(registry, tmap.get_char_on_cursor(registry), "anim");
        
        sol::table characters = scripts->lua["Characters"];
        char_hover_data.char_name = char_name;
        char_hover_data.label_name = "Name: " + char_hover_data.char_name;
        char_hover_data.profile_pic = resources->get_profile_pic(char_hover_data.char_name);
    }
    
}

void StartState::show_character_hover_ui(nk_context* ctx) {

    if (nk_begin(ctx, "CHARACTER", nk_rect(100, 100, 300, 500), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
        
        nk_layout_row_static(ctx, 200, 200, 1);
        nk_image(ctx, char_hover_data.profile_pic);

        nk_layout_row_static(ctx, 10, 200, 4);

        nk_label(ctx, char_hover_data.label_name.c_str(), NK_TEXT_LEFT);
    }
    nk_end(ctx);
}