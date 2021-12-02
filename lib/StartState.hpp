#pragma once

#include "State.hpp"

#include <AudioSystem.hpp>
#include "Injector.hpp"
#include "TileMap.hpp"
#include "WorldCacheSystem.hpp"
#include "glm/glm.hpp"

class StartState : public State {

    static constexpr int grid_width = 3;
    static constexpr int grid_height = 3;
    static constexpr int player_id = 12;

    enum class next_state {
        game_state,
        credits_state
    };
    TileMap2D tmap;
    std::unique_ptr<WorldCacheSystem> world;
    /**
     * @brief add to position vector
    */
    glm::vec3 move_vec(int x, int y);

    /**
     * @brief move in screen and cache
    */
    void move(entt::registry& registry, int x, int y);

    template <int x, int y>
    void pan();

    void build_key_handlers();
    void build_mouse_handlers();
    void build_music();
    void build_gfx();
    void build_scene(entt::registry& registry);
    
    public:
        using State::State;
        StartState(std::shared_ptr<Injector> injector, entt::registry& registry){
            this->injector = injector;
            key_system = {};
            mouse_system = {injector};

            build_gfx();
            build_key_handlers();
            build_mouse_handlers();
            build_music();
            
            build_scene(registry);
        };
        void process_systems(entt::registry& registry);
        std::shared_ptr<State> next(entt::registry& registry);
};