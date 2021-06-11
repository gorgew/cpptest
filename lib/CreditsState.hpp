#pragma once

#include "State.hpp"

#include "Injector.hpp"

class CreditsState : public State {

    public:
        CreditsState(std::shared_ptr<Injector> injector, entt::registry& registry);
        void process_systems(entt::registry& registry);
        std::shared_ptr<State> next(entt::registry& registry);
};