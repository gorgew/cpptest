#include "StartState.hpp"

StartState::StartState(std::shared_ptr<Injector> injector) {
    this->injector = injector;
    
    //Key handlers
    key_system = {};

    //Mouse handlers
    mouse_system = {injector};

    //Other systems
}

void StartState::process_systems(entt::registry& registry) {

    key_system.execute_holds(registry);
}