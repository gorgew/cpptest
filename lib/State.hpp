#pragma once

#include <entt/entt.hpp>
#include "KeyEventSystem.hpp"
//Mouse thingy once I fix that
//All the systems the state uses
class State {
    
    public:
        //Initailize the base stuff
        State();
        void process_systems(entt::registry& registry);
        State next();      
};