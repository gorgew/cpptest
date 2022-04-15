#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

struct physics {
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

class PhysicsSystem {
    public:
        void process_physics(entt::registry&, float delta_time);
};