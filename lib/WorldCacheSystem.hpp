#pragma once

#include "GameObjectComponents.hpp"
#include <entt/entt.hpp>
#include <vector>

class WorldCacheSystem {
    
    unsigned int x;
    unsigned int y;
    unsigned int z;

    entt::observer observer;
    
    void resize(unsigned int x, unsigned int y, unsigned int z);
    void add_observers(entt::registry& registry);
    void add_handlers(entt::registry& registry);
    void destruction_handler(entt::registry&, entt::entity);
    
    public:
        std::vector<std::vector<entt::entity>> environment_map; //map of environment entities
        std::vector<std::vector<entt::entity>> character_map; //map of character entities
        std::vector<std::vector<entt::entity>> terrain_map; //map of terrain entities
        //Dimensions of world
        
        WorldCacheSystem(entt::registry& registry, unsigned int x, unsigned int y, unsigned int z);
        entt::entity get(unsigned int x, unsigned int y, unsigned z);
        void load(entt::registry& registry);
        void observe(entt::registry& registry);
};