#include "StartState.hpp"

#include <fmt/format.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GraphicsComponents.hpp>
#include <PhysicsComponents.hpp>
#include "TileMap.hpp"
#include "WorldCacheSystem.hpp"
#include <color-vec.hpp>

#include "CreditsState.hpp"

void StartState::build_key_handlers() {
    fmt::print("Building key handlers\n");
    /*
    std::function<void(entt::registry&)> move_right= [=, this](entt::registry& registry) {
        move(registry, 1, 0);
    };

    std::function<void(entt::registry&)> move_left= [=, this](entt::registry& registry) {
        move(registry, -1, 0);
    };
    std::function<void(entt::registry&)> move_up= [=, this](entt::registry& registry) {
        move(registry, 0, 1);
    };
    std::function<void(entt::registry&)> move_down = [=, this](entt::registry& registry) {
        move(registry, 0, -1);
    };

    std::function<void(entt::registry&)> next_n = [=, this](entt::registry& registry) {
        go_next = true;
    };

    key_system.add_keydown_handler(SDLK_d, move_right);
    key_system.add_keydown_handler(SDLK_a, move_left);
    key_system.add_keydown_handler(SDLK_w, move_up);
    key_system.add_keydown_handler(SDLK_s, move_down);
    //key_system.add_keydown_handler(SDLK_n, next_n);
    */

   std::function<void(entt::registry&)> pan_right = [&](entt::registry& registry) mutable {
        camera.pan(-1, -1);
    };

    std::function<void(entt::registry&)> pan_left = [=, this](entt::registry& registry) {
        camera.pan(1, 1);
    };
    std::function<void(entt::registry&)> pan_up = [=, this](entt::registry& registry) {
        camera.pan(1, -1);
    };
    std::function<void(entt::registry&)> pan_down = [=, this](entt::registry& registry) {
        camera.pan(-1, 1);
    };
    std::function<void(entt::registry&)> placeholder = [](entt::registry&){

    };
    key_system.add_held_key_handler(SDLK_d, pan_right, placeholder);
    key_system.add_held_key_handler(SDLK_a, pan_left, placeholder);
    key_system.add_held_key_handler(SDLK_w, pan_up, placeholder);
    key_system.add_held_key_handler(SDLK_s, pan_down, placeholder);
    
}

void StartState::build_mouse_handlers() {
    fmt::print("Building mouse handlers");

    std::function<void(entt::registry&)> zoom_in = [=, this](entt::registry& registry) {
        camera.zoom(1);
    };
    std::function<void(entt::registry&)> zoom_out = [=, this](entt::registry& registry) {
        camera.zoom(-1);
    };
    
    mouse_system.add_wheel_handler(true, zoom_in);
    mouse_system.add_wheel_handler(false, zoom_out);
}

void StartState::build_music() {
    fmt::print("Building music\n");
    injector->audio.add_music("dd-town", "../resources/dd-town.wav");
    injector->audio.enqueue_music("dd-town");
    //registry.emplace<audio_request>(music_command, "dd-town");

    injector->audio.add_effect("slow-killer", "../resources/slow_killer.wav");
    
    std::function<void(entt::registry&)> play_on_y= [=, this](entt::registry& registry) {
        injector->audio.enqueue_effect("slow-killer");
    };
    key_system.add_keydown_handler(SDLK_y, play_on_y);
}

void StartState::build_gfx() {
    fmt::print("Building gfx\n");
    //Other systems
    injector->tex_man.add_2d_array_texture("blank", "../resources/NumsPacked.png", 32, 32, 6);
    injector->shader_man.add_shader("sprites-v", "../resources/sprites.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("sprite-f", "../resources/sprite.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("sprites", {"sprites-v", "sprite-f"});
    injector->shader_man.use("sprites");
    program_id  = injector->shader_man.get_program_id("sprites");

    glUniform1i(glGetUniformLocation(program_id, "iTexture"), 0);
    
    camera = {static_cast<float>(injector->config.width), 
        static_cast<float>(injector->config.height),
        1.0f, 
        0.1f,
        program_id};
    /*
    view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(-35.264f), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, glm::vec3(100.0f, 500.0f, 0.0f));
    */

    auto light_color = color_vec::make_rgb(255, 255, 100, 1.0f);
    glUniform3fv(glGetUniformLocation(program_id, "light_color"), 1, light_color);

    /*
    injector->tex_man.add_2d_array_texture("dummy-menu", "../resources/dummy-menu.png", 128, 128, 2);
    injector->tex_man.add_2d_array_texture("tiles", "../resources/iso-h-v1.png", 16, 16, 4);
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
    */

    /*
    
    struct array_frame my_tile = gorge::build_array_frame(injector, 100.0f, 100.0f, "tiles", 1, "sprites");
    const auto tile = registry.create();
    registry.emplace<array_frame>(tile, my_tile);
    registry.emplace<position>(tile, glm::vec3(400.0f, 200.0f, 0.0f));
    */ 
    injector->tex_man.add_2d_array_texture("art", "../resources/programmer-art.png", 16, 16, 12);
    std::vector<std::vector<int>> terrain_arr = {
        {1, 2, 3}, 
        {2, 3, 3},
        {0, 3, 3}
    };
    std::vector<std::vector<int>> env_arr = {
        {4, 4, -1}, 
        {-1, -1, -1},
        {7, -1, -1}
    };
    std::vector<std::vector<int>> char_arr = {
        {-1, -1, -1}, 
        {-1, -1, 12},
        {-1, -1, -1}
    };

    tmap = {injector, "art", "sprites"};
    tmap.add_tiles(registry, terrain_arr, env_arr, char_arr);

    world = std::make_unique<WorldCacheSystem>(registry, grid_width, grid_height, 1);

    std::function<void(entt::registry&)> shift_down = [](entt::registry& registry) {
        auto view = registry.view<position>();
    
        fmt::print("surprise\n");
    };
    const auto button = registry.create();
    registry.emplace<position>(button, glm::vec3(400.0f, 400.0f, 0.0f));
    registry.emplace<rect_button>(button, 800.0f, 800.0f, shift_down);
};

void StartState::process_systems(entt::registry& registry) {
    if (systems_enabled) {
        key_system.execute_holds(registry);
        world->observe(registry);
    }
}

std::shared_ptr<State> StartState::next(entt::registry& registry) {

    registry.clear();
    return std::make_shared<CreditsState>(injector, registry);
}

void StartState::move(entt::registry& registry, int x, int y) {

    auto view = registry.view<position, character, world_pos>();
    for (auto [e, pos, character, w_pos] : view.each()) {
        if (character.type_id == player_id) {
            registry.emplace_or_replace<old_world_pos>(e, w_pos.x, w_pos.y, w_pos.z);
            w_pos.x += x;
            w_pos.y += y;
            pos.pos += move_vec(x, y);
        }
    }
}

glm::vec3 StartState::move_vec(int x, int y) {
    return glm::vec3(injector->config.tile_width * x, injector->config.tile_height * y, 0.0f);
}

void StartState::resize(int x, int y) {
    camera.resize(x, y);
}