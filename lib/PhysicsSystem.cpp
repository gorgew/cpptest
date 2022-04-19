#include "PhysicsSystem.hpp"
#include "GraphicsComponents.hpp" 
#include "PhysicsComponents.hpp"

void PhysicsSystem::process_physics(entt::registry& r, float delta_time) {
    auto view = r.view<position, physics>();
    for (auto [e, pos, phys] : view.each()) {
        phys.velocity += (phys.acceleration * delta_time);
        pos.pos += (phys.velocity * delta_time);
    }
}