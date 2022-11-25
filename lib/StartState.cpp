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

StartState* StartState::Substate::s = nullptr;

void StartState::Substate::handle_event(entt::registry& registry, SDL_Event e) {
    
    key_system.handle_event(registry, e);
    mouse_system.handle_event(registry, e);
    
}

StartState::ObserveSubstate& StartState::ObserveSubstate::instance() {
    static ObserveSubstate instance;
    return instance;
}

void StartState::ObserveSubstate::build_key_handlers() {
    fmt::print("BUILD OBSERVE STATE HANDLERS\n");
    std::function<void(entt::registry&)> pan_right = [&](entt::registry& registry) mutable {
        fmt::print("PANRIGHT\n");
        s->camera->pan(1, 0, 0, s->delta_time);
    };

    key_system.add_held_key_handler(SDLK_d, pan_right, placeholder);
    key_system.add_held_key_handler(SDLK_a, pan_left, placeholder);
    key_system.add_held_key_handler(SDLK_w, pan_up, placeholder);
    key_system.add_held_key_handler(SDLK_s, pan_down, placeholder);    

    std::function<void(entt::registry&)> pause = [&](entt::registry& registry) mutable {
        fmt::print("TODO add pause menu\n");
    };
    key_system.add_keydown_handler(SDLK_ESCAPE, pause);
}

void StartState::ObserveSubstate::build_mouse_handlers() {
    fmt::print("observestate mnouse\n");
    std::function<void(entt::registry&)> move_cursor = [=, this](entt::registry& registry) {
        s->tmap.move_cusor(mouse_system.world_x, mouse_system.world_y);
        if (s->tmap.get_char_on_cursor() != entt::null) {
            s->ui_show_character_hover = true;
            s->update_char_hover_data(registry);
        }
        else {
            s->ui_show_character_hover = false;
        }
    };

    std::function<void(entt::registry&)> l_mouse_down = [=, this](entt::registry& registry) {
        if (s->ui_show_character_hover) {
            s->tmap.add_player_range_cursor(
                (int) s->locator.get_scripts()->lua["Characters"][s->tmap.last_char]["stats"]["movement"]);
            s->setState(CharacterSelected::instance());
        }
    };
   
    mouse_system.set_motion_handler(move_cursor);
    mouse_system.add_mousedown_handler(l_mouse_down, SDL_BUTTON_LEFT);
   
}

StartState::CharacterSelected& StartState::CharacterSelected::instance() {
    static CharacterSelected instance;
    return instance;
}

void StartState::CharacterSelected::build_key_handlers() {
    key_system.add_held_key_handler(SDLK_d, pan_right, placeholder);
    key_system.add_held_key_handler(SDLK_a, pan_left, placeholder);
    key_system.add_held_key_handler(SDLK_w, pan_up, placeholder);
    key_system.add_held_key_handler(SDLK_s, pan_down, placeholder); 

    std::function<void(entt::registry&)> exit = [=, this](entt::registry& registry) {
        s->tmap.clear_player_range();
        s->tmap.clear_path();
        s->setState(ObserveSubstate::instance());
    };
    key_system.add_keydown_handler(SDLK_1, exit); 
}

void StartState::CharacterSelected::build_mouse_handlers() {  
    std::function<void(entt::registry&)> move_cursor = [=, this](entt::registry& registry) {
        s->tmap.move_cursor_path(mouse_system.world_x, mouse_system.world_y);
    }; 
    mouse_system.set_motion_handler(move_cursor);

    std::function<void(entt::registry&)> r_mouse_down = [=, this](entt::registry& registry) {
        if (s->tmap.move_character_selected_cursor()) {
            auto time = s->action_seq.set_path_walk(registry, s->tmap.get_char_on_cursor(), 
                s->tmap.stored_path, 0.2f);
            s->tmap.clear_player_range();
            s->tmap.clear_path();
            s->set_lockout(time);
            fmt::print("MOVED\n");
            s->setState(ObserveSubstate::instance());
        }
        else {
            fmt::print("DIDNI't MOVE\n");
        }
        
    };

    mouse_system.add_mousedown_handler(r_mouse_down, SDL_BUTTON_RIGHT);
    
}

StartState::SkillSelected& StartState::SkillSelected::instance() {
    static SkillSelected instance;
    return instance;
}

void StartState::SkillSelected::build_key_handlers() {
    key_system.add_held_key_handler(SDLK_d, pan_right, placeholder);
    key_system.add_held_key_handler(SDLK_a, pan_left, placeholder);
    key_system.add_held_key_handler(SDLK_w, pan_up, placeholder);
    key_system.add_held_key_handler(SDLK_s, pan_down, placeholder); 

    std::function<void(entt::registry&)> exit = [=, this](entt::registry& registry) {
        s->tmap.clear_player_range();
        s->tmap.clear_path();
        s->setState(ObserveSubstate::instance());
    };
    key_system.add_keydown_handler(SDLK_1, exit); 
}

void StartState::SkillSelected::build_mouse_handlers() {  
    std::function<void(entt::registry&)> move_cursor = [=, this](entt::registry& registry) {
        s->tmap.move_cursor_path(mouse_system.world_x, mouse_system.world_y);
    }; 
    mouse_system.set_motion_handler(move_cursor);

    std::function<void(entt::registry&)> r_mouse_down = [=, this](entt::registry& registry) {
        
    };

    mouse_system.add_mousedown_handler(r_mouse_down, SDL_BUTTON_RIGHT);
    
}

void StartState::setState(Substate& s) {
    _substate->exit(this);
    _substate = &s;
    _substate->enter(this);
}

void StartState::build_key_handlers() {
    /*
    std::function<void(entt::registry&)> placeholder = [](entt::registry&){

    };
    
    std::function<void(entt::registry&)> pitch_left = [=, this](entt::registry& registry) {
        camera->update_pitch(-1.0f);
    };
    std::function<void(entt::registry&)> pitch_right = [=, this](entt::registry& registry) {
        camera->update_pitch(1.0f);
    };
    system.add_held_key_handler(SDLK_s, pan_down, placeholder);
    d::function<void(entt::registry&)> yaw_down = [=, this](entt::registry& registry) {
        camera->update_yaw(1.0f);
    };
    std::function<void(entt::registry&)> yaw_up = [=, this](entt::registry& registry) {
        camera->update_yaw(-1.0f);
    };
    key_system.add_held_key_handler(SDLK_q, pitch_left, placeholder);
    key_system.add_held_key_handler(SDLK_e, pitch_right, placeholder);
    key_system.add_held_key_handler(SDLK_z, yaw_up, placeholder);
    key_system.add_held_key_handler(SDLK_x, yaw_down, placeholder);
*/
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
         if (m_substate == substate::skill_target) {
            tmap.move_cusor(mouse_system.world_x, mouse_system.world_y);
            if (tmap.get_char_on_cursor() != entt::null) {
                //show target / stats hover
            }
        }

        if (tmap.get_env_on_cursor() != entt::null) {
            ui_show_env = true;
        }
        else {
            ui_show_env = false;
        }
        if (tmap.get_terrain_on_cursor() != entt::null) {
            ui_show_terrain = true;
        }
        else {
            ui_show_terrain = false;
        }
    };

    std::function<void(entt::registry&)> l_mouse_down = [=, this](entt::registry& registry) {
        if (ui_show_character_hover && m_substate == substate::observe_world) {
            tmap.add_player_range_cursor(
                (int) locator.get_scripts()->lua["Characters"][tmap.last_char]["stats"]["movement"]);
            m_substate = substate::character_select;
        }
        else if (m_substate == substate::character_select) {
            
            tmap.clear_player_range();
            tmap.clear_path();
            m_substate = substate::observe_world;
            ui_show_character_hover = false;
        }
    };

    mouse_system.add_mousedown_handler(l_mouse_down, SDL_BUTTON_LEFT);
}

void StartState::build_music() {
    /*
    locator.get_audio()->add_music("dd-town", "resources/dd-town.wav");
    locator.get_audio()->enqueue_music("dd-town");
    //registry.emplace<audio_request>(music_command, "dd-town");

    locator.get_audio()->add_effect("slow-killer", "resources/slow_killer.wav");
    
    std::function<void(entt::registry&)> play_on_y= [=, this](entt::registry& registry) {
        locator.get_audio()->enqueue_effect("slow-killer");
    };
    key_system.add_keydown_handler(SDLK_y, play_on_y);
    */
}

void StartState::build_gfx() {
    fmt::print("Building gfx\n");
    //Other systems
    locator.get_textures()->add_2d_array_texture("blank", "resources/NumsPacked.png", 32, 32, 6);
    locator.get_shaders()->use("world");
    program_id  = locator.get_shaders()->get_program_id("world");

    //sglUniform1i(glGetUniformLocation(program_id, "iTexture"), 0);
    //glUniform1i(glGetUniformLocation(locator.get_shaders()->get_program_id("billboard"), "iTexture"), 0);

    locator.get_shaders()->add_ubo("camera_ubo", sizeof(camera_data), 0);
    locator.get_shaders()->bind_ubo("world", "camera_ubo", 0);

    camera.reset(new Camera(static_cast<float>(locator.get_config()->width), 
        static_cast<float>(locator.get_config()->height),
        100.0f, 
        1.0f,
        locator.get_shaders()->get_ubo("camera_ubo")));


    //auto light_color = color_vec::make_rgb(255, 255, 100, 1.0f);
    //glUniform3fv(glGetUniformLocation(program_id, "light_color"), 1, light_color);

    /*
    locator.get_textures()->add_2d_array_texture("dummy-menu", "resources/dummy-menu.png", 128, 128, 2);
    locator.get_textures()->add_2d_array_texture("tiles", "resources/iso-h-v1.png", 16, 16, 4);
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

    struct array_frame my_tile = gorge::build_array_frame(100.0f, 100.0f, "tiles", 1, "sprites");
    const auto tile = registry.create();
    registry.emplace<array_frame>(tile, my_tile);
    registry.emplace<position>(tile, glm::vec3(400.0f, 200.0f, 0.0f));
    */ 
   {
        std::vector<int> frames = {0, 1, 2, 3};
        std::vector<int> timings = {1000, 1000, 1000, 1000};
        locator.get_resources()->add_animation_texture("anim", "resources/anim.png", 16, 16, 4, 100, 100, "billboard",
            true, frames, timings);
   }
    
    tmap.load_tileset(locator.get_scripts()->lua);
    tmap.load_tiles(locator.get_scripts()->lua);
    tmap.load_map("Test", locator.get_scripts()->lua);

    std::function<void(entt::registry&)> shift_down = [](entt::registry& registry) {
    
        fmt::print("surprise\n");
    };
    /*
    const auto button = registry.create();
    registry.emplace<position>(button, glm::vec3(400.0f, 400.0f, 0.0f));
    registry.emplace<rect_button>(button, 800.0f, 800.0f, shift_down);
    */
};

void StartState::handle_event(entt::registry& registry, SDL_Event e) {
    _substate->handle_event(registry, e);
    //key_system.handle_event(registry, e);
    //mouse_system.handle_event(registry, e);
}

void StartState::set_lockout(float time) {
    m_substate = substate::lockout;
    lockout_timer = time;
}

void StartState::lockout_transition(float& time) {
    if (lockout_timer != 0) {
        lockout_timer -= time;
        fmt::print("lockout time: {}\n", lockout_timer);
        if (lockout_timer <= 0) {
            lockout_timer = 0;
            m_substate = substate::observe_world;
        }
    }
}

void StartState::process_systems(entt::registry& registry, float& delta_time) {
    if (systems_enabled) {
        key_system.execute_holds(registry);
        _substate->key_system.execute_holds(registry);
        lockout_transition(delta_time);
        action_seq.process(registry, delta_time);
    }
}

std::shared_ptr<State> StartState::next(entt::registry& registry) {

    registry.clear();
    return std::make_shared<CreditsState>(registry);
}

void StartState::resize(int x, int y) {
    camera->resize(x, y);
    locator.get_config()->width = x;
    locator.get_config()->height = y;
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
    if (ui_show_env) {
        show_env_ui(ctx);
    }
    else if (ui_show_terrain) {
        show_terrain_ui(ctx);
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

        //locator.get_resources()->set_animation(registry, tmap.get_char_on_cursor(registry), "anim");
        
        sol::table characters = locator.get_scripts()->lua["Characters"];
        char_hover_data.char_name = char_name;
        char_hover_data.label_name = "Name: " + char_hover_data.char_name;
        char_hover_data.profile_pic = locator.get_resources()->get_profile_pic(char_hover_data.char_name);

        char_hover_data.active_skills.clear();
        char_hover_data.passive_skills.clear();
        sol::table active_skills = characters[char_name]["active_skills"];
        sol::table passive_skills = characters[char_name]["passive_skills"];
        if (char_name == "Bob") {
            fmt::print("asdfasdf {}\n", active_skills.size());
        }
        for (const auto& skill : active_skills) {
            char_hover_data.active_skills.push_back(skill.second.as<std::string>());
        }
        for (const auto& skill : passive_skills) {
            char_hover_data.passive_skills.push_back(skill.second.as<std::string>());
        }
    }
    
}

void StartState::show_character_hover_ui(nk_context* ctx) {

    if (nk_begin(ctx, "CHARACTER", nk_rect(0, 0, 300, 300), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
        
        nk_layout_row_static(ctx, 200, 200, 1);
        nk_image(ctx, char_hover_data.profile_pic);

        nk_layout_row_static(ctx, 10, 200, 1);

        nk_label(ctx, char_hover_data.label_name.c_str(), NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "Active Skills", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, char_hover_data.active_skills.size());
        for (const auto& s: char_hover_data.active_skills) {
            if (nk_button_label(ctx, s.c_str())) {
                auto skill = locator.get_scripts()->lua["ActiveSkills"][s.c_str()];
                
                if (skill["isAttack"]) {
                    setState(SkillSelected::instance());
                    skill_indicator = skill_indicator_t::path;
                    tmap.clear_path();
                    tmap.clear_player_range();
                    tmap.add_player_range(tmap.last_char_x, tmap.last_char_y, 1, false, range_t::enemy);
                    skill_indicator = skill_indicator_t::melee;
                }

                fmt::print("Use active skill {}\n", s);
            }
        }

    }
    nk_end(ctx);
}

void StartState::show_env_ui(nk_context* ctx) {
    if (nk_begin(ctx, "", nk_rect(0, 800, 300, 100), NK_WINDOW_BORDER)) {
        nk_layout_row_static(ctx, 100, 200, 1);
        nk_label(ctx, tmap.last_env.c_str(), NK_TEXT_LEFT);
    }
    nk_end(ctx);
}   

void StartState::show_terrain_ui(nk_context* ctx) {
    if (nk_begin(ctx, "", nk_rect(0, 800, 300, 100), NK_WINDOW_BORDER)) {
        nk_layout_row_static(ctx, 100, 200, 1);
        nk_label(ctx, tmap.last_terrain.c_str(), NK_TEXT_LEFT);
    }
    nk_end(ctx);
}