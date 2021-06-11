#include "StartState.hpp"

#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GraphicsComponents.hpp>
#include <PhysicsComponents.hpp>
#include "AudioComponent.hpp"

#include "CreditsState.hpp"

StartState::StartState(std::shared_ptr<Injector> injector, entt::registry& registry) {
    this->injector = injector;

    //Key handlers
    key_system = {};

    std::function<void(entt::registry&)> shift_right= [](entt::registry& registry) {
        auto view = registry.view<position>();
        for (auto [entity, pos] : view.each()) {
            pos.pos = pos.pos + glm::vec3(10.0f, 0.0f, 0.0f);
        }
    };

    std::function<void(entt::registry&)> next_n = [=, this](entt::registry& registry) {
        go_next = true;
    };

    key_system.add_keydown_handler(SDLK_a, shift_right);
    key_system.add_keydown_handler(SDLK_n, next_n);

    //Mouse handlers
    mouse_system = {injector};

    //Other systems
    injector->tex_man.add_2d_array_texture("blank", "../resources/NumsPacked.png", 32, 32, 6);
    injector->shader_man.add_shader("sprites-v", "../resources/sprites.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("array-tex", "../resources/array-tex.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("sprites", {"sprites-v", "array-tex"});
    injector->shader_man.use("sprites");
    int id = injector->shader_man.get_program_id("sprites");
    glUniform1i(glGetUniformLocation(id, "iTexture"), 0);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(injector->config.width), 
            0.0f, static_cast<float>(injector->config.height));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    injector->tex_man.add_2d_array_texture("dummy-menu", "../resources/dummy-menu.png", 128, 128, 2);
    
    struct array_frame mf = gorge::build_array_frame(injector, 400.0f, 400.0f, "dummy-menu", 0, "sprites");
    const auto menu = registry.create();
    registry.emplace<array_frame>(menu, mf);
    registry.emplace<position>(menu, glm::vec3(400.0f, 400.0f, 0.0f));
    
    struct array_frame my_tile = gorge::build_array_frame(injector, 400.0f, 400.0f, "dummy-menu", 1, "sprites");
    const auto tile = registry.create();
    registry.emplace<array_frame>(tile, my_tile);
    registry.emplace<position>(tile, glm::vec3(400.0f, 400.0f, 0.0f));
    
    struct array_frame_node my_animated_graphic = gorge::build_array_frame_node(injector, 200.0f, 200.0f, 
            "blank", 0, 5, "sprites");
    const auto entity = registry.create();
    registry.emplace<array_frame_node>(entity, my_animated_graphic);
    registry.emplace<position>(entity, glm::vec3(200.0f, 200.0f, 0.0f));

    /*
    injector->tex_man.add_2d_array_texture("tiles", "../resources/iso-h-v1.png", 16, 16, 4);
    struct array_frame my_tile = gorge::build_array_frame(injector, 100.0f, 100.0f, "tiles", 1, "sprites");
    const auto tile = registry.create();
    registry.emplace<array_frame>(tile, my_tile);
    registry.emplace<position>(tile, glm::vec3(400.0f, 200.0f, 0.0f));
    */ 
   std::function<void(entt::registry&)> shift_down= [](entt::registry& registry) {
        auto view = registry.view<position>();
    
        fmt::print("surprise\n");
    };
    const auto button = registry.create();
    registry.emplace<position>(button, glm::vec3(400.0f, 400.0f, 0.0f));
    registry.emplace<rect_button>(button, 800.0f, 800.0f, shift_down);
    
    injector->audio.add_music("dd-town", "../resources/dd-town.wav");
    const auto music_command = registry.create();
    registry.emplace<audio_request>(music_command, "dd-town");

    injector->audio.add_effect("slow-killer", "../resources/slow_killer.wav");
    
    std::function<void(entt::registry&)> play_on_y= [=, this](entt::registry& registry) {
        const auto sound = registry.create();
        registry.emplace<audio_request>(sound, "slow-killer");
    };
    key_system.add_keydown_handler(SDLK_y, play_on_y);
}

void StartState::process_systems(entt::registry& registry) {
    key_system.execute_holds(registry);
}

std::shared_ptr<State> StartState::next(entt::registry& registry) {

    registry.clear();
    return std::make_shared<CreditsState>(injector, registry);
}