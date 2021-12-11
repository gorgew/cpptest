#include "CreditsState.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GraphicsComponents.hpp>
#include <PhysicsComponents.hpp>

CreditsState::CreditsState(std::shared_ptr<Injector> injector, entt::registry& registry) {
    this->injector = injector;
    
    //Key handlers
    key_system = {};

    //Mouse handlers
    mouse_system = {injector};

    //Other systems
}

void CreditsState::process_systems(entt::registry& registry) {

    key_system.execute_holds(registry);
}

std::shared_ptr<State> CreditsState::next(entt::registry& registry) {

    registry.clear();
    return 0;
}

void CreditsState::build_handlers() {
    
}

void CreditsState::load_music() {
    
}

void CreditsState::load_graphics() {
    
}

void CreditsState::initialize_scene() {
    
}

void CreditsState::resize(int x, int y) {
    
}