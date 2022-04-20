#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <robin_hood/robin_hood.h>
#include <utility>
#include <queue>
#include <string>
#include "PhysicsSystem.hpp"
#include "GameObjectComponents.hpp"
#include "ResourceManager.hpp"
#include "Injector.hpp"

struct action_instruction {
    float time;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

/**
 * Controls physics and animations of entities
*/
class ActionSequencer {

    struct instruction {
        float time;
        std::string animation;
        glm::vec3 velocity;
        glm::vec3 acceleration;
    };

    robin_hood::unordered_map<entt::entity, std::queue<instruction>> instruction_queue;
    Locator locator;
    
    public:
        void process(entt::registry& registry, float& delta_time);
        /*
        Set path in along tiles.
        Return time taken.
        */
        float set_path(entt::registry&, entt::entity, std::vector<std::pair<direction, float>>& path, float time);

        /*
        Set path but also change to walking animations based off of direction
        */
        float set_path_walk(entt::registry&, entt::entity, std::string char_name,
            std::vector<std::pair<direction, float>>& path, float time);
};