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

/**
 * Controls physics and animations of entities
*/
class ActionSequencer {

    struct instruction {
        std::string animation;
        glm::vec3 velocity;
        glm::vec3 acceleration;
    };

    robin_hood::unordered_map<entt::entity, std::queue<std::pair<float, instruction>>> instruction_queue;
    std::shared_ptr<ResourceManager> resources;
    public:
        ActionSequencer(std::shared_ptr<ResourceManager> resources) : resources(resources) {}
        void process(entt::registry& registry, float& delta_time);
        void set_path(entt::entity, std::vector<std::pair<direction, float>>& path, float time);
};