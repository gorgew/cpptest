#include "WorldCacheSystem.hpp"

#include <ranges>

namespace ranges = std::ranges;

WorldCacheSystem::WorldCacheSystem(entt::registry& registry, unsigned int x, unsigned int y, unsigned int z) {
    this->x = x;
    this->y = y;
    this->z = z;

    resize(x, y, z);
    load(registry);
    add_observers(registry);
    add_handlers(registry);
}

void WorldCacheSystem::resize(unsigned int x, unsigned int y, unsigned int z) {

    //Resize x's
    terrain_map.resize(x);
    environment_map.resize(x);
    character_map.resize(x);

    //Resize y's
    for (unsigned int i = 0; i < x; ++i) {
        terrain_map[i].resize(y);
    }
    for (unsigned int i = 0; i < x; ++i) {
        environment_map[i].resize(y);
    }
    for (unsigned int i = 0; i < x; ++i) {
        character_map[i].resize(y);
    }

    //Resize z's
    for (unsigned int i = 0; i < x; ++i) {
        for (unsigned int j = 0; j < y; ++j) {
            terrain_map[i][j].resize(z);
        }
    }
    for (unsigned int i = 0; i < x; ++i) {
        for (unsigned int j = 0; j < y; ++j) {
            environment_map[i][j].resize(z);
        }
    }
    for (unsigned int i = 0; i < x; ++i) {
        for (unsigned int j = 0; j < y; ++j) {
            character_map[i][j].resize(z);
        }
    }

    for (unsigned int i = 0; i < x; ++i) {
        for (unsigned int j = 0; j < y; ++j) {
            for (unsigned int k = 0; k < z; ++k) {
                terrain_map[i][j][k] = entt::null;
                environment_map[i][j][k] = entt::null;
                character_map[i][j][k] = entt::null;
            }
        }
    }
}

void WorldCacheSystem::load(entt::registry& registry) {

    auto terrain_view = registry.view<terrain, world_pos>();
    for (auto [e, type, pos] : terrain_view.each()) {
        terrain_map[pos.x][pos.y][pos.z] = e;
    }
    auto environment_view = registry.view<environment, world_pos>();
    for (auto [e, type, pos] : environment_view.each()) {
        environment_map[pos.x][pos.y][pos.z] = e;
    }
    auto character_view = registry.view<character, world_pos>();
    for (auto [e, type, pos] : character_view.each()) {
        character_map[pos.x][pos.y][pos.z] = e;
    }
}

void WorldCacheSystem::add_observers(entt::registry& registry) {
    observer.connect(registry, entt::collector.update<world_pos>());
}

void WorldCacheSystem::add_handlers(entt::registry& registry) {
    registry.on_destroy<world_pos>().connect<&WorldCacheSystem::destruction_handler>(this);
}

void WorldCacheSystem::destruction_handler(entt::registry& registry, entt::entity entity) {
    auto pos = registry.get<world_pos>(entity);
    auto x = registry.try_get<environment>(entity);
    auto y = registry.try_get<character>(entity);
    if (x) {
        environment_map[pos.x][pos.y][pos.z] = entt::null;
    }
    else if (y){
        character_map[pos.x][pos.y][pos.z] = entt::null;
    }
    if (registry.all_of<environment>(entity)) {
        environment_map[pos.x][pos.y][pos.z] = entt::null;
    }
    else if (registry.all_of<character>(entity)) {
        character_map[pos.x][pos.y][pos.z] = entt::null;
    }
    else {
        //character_map[pos.x][pos.y][pos.z] = entt::entity(70);
    }

}

void WorldCacheSystem::observe(entt::registry& registry) {

    for (const auto entity : observer) {
        auto [pos, old_pos] = registry.get<world_pos, old_world_pos>(entity);
        if (registry.all_of<environment>(entity)) {
            environment_map[old_pos.x][old_pos.y][old_pos.z] = entt::null;
            environment_map[pos.x][pos.y][pos.z] = entity;
            
        }
        else if (registry.all_of<character>(entity)) {
            character_map[old_pos.x][old_pos.y][old_pos.z] = entt::null;
            character_map[pos.x][pos.y][pos.z] = entity;
        }
        else {
            character_map[pos.x][pos.y][pos.z] = entt::entity(69);
        }
    }
    observer.clear();
}