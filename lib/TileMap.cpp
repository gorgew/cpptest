#include "TileMap.hpp"
#include "GraphicsComponents.hpp"
#include "PhysicsComponents.hpp"
#include "GameObjectComponents.hpp"
#include <fmt/printf.h>

TileMap2D::TileMap2D(std::shared_ptr<Injector> injector,std::string tex_name, std::string terrain_shader, 
    std::string character_shader) {    

    this->injector = injector;
    tile_width = injector->config.tile_width;
    tile_height = injector->config.tile_height;
    this->tex_name = tex_name;
    this->terrain_shader = terrain_shader;
    this->character_shader = character_shader;
}

void TileMap2D::add_tiles(entt::registry& registry, std::vector<std::vector<int>> terrain_arr,
                std::vector<std::vector<int>> env_arr, std::vector<std::vector<int>> char_arr) {
    
    cursor = registry.create();
    auto cursor_frame = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, cursor_id, terrain_shader);
    registry.emplace<array_frame>(cursor, cursor_frame);
    registry.emplace<ui>(cursor);
    registry.emplace<position>(cursor, glm::vec3(0.0f, 0.0f, 0.0f));

    {
        int y = terrain_arr.size();
        int x = terrain_arr[0].size();
        terrain_cache.resize(y);
        for (int i = 0; i < y; i++) {
            terrain_cache[i].resize(x);
            for (int j = 0; j < x; j++) {
                terrain_cache[i][j] = entt::null;
            }
        }
    }
    {
        int y = env_arr.size();
        int x = env_arr[0].size();
        env_cache.resize(y);
        for (int i = 0; i < y; i++) {
            env_cache[i].resize(x);
            for (int j = 0; j < x; j++) {
                env_cache[i][j] = entt::null;
            }
        }

    }
    {
        int y = char_arr.size();
        int x = char_arr[0].size();
        char_cache.resize(y);
        for (int i = 0; i < y; i++) {
            char_cache[i].resize(x);
            for (int j = 0; j < x; j++) {
                char_cache[i][j] = entt::null;
            }
        }
    }
    //Stick in registry

    for (size_t i = 0; i < terrain_arr.size(); i ++) {
        for (size_t j = 0; j < terrain_arr[i].size(); j++) {
            if (terrain_arr[i][j] >= 0) {
                auto entity = registry.create();
                struct array_frame arr_f = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, terrain_arr[i][j], terrain_shader);
                registry.emplace<array_frame>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width / 2.0f + j * tile_width, 
                        tile_height / 2.0f + i * tile_height, 0.0f));

                //CHANGE LATER
                registry.emplace<terrain>(entity, 1);
                terrain_cache[j][i] = entity;
            }
        }
    }

    for (size_t i = 0; i < env_arr.size(); i ++) {
        for (size_t j = 0; j < env_arr[i].size(); j++) {
            if (env_arr[i][j] >= 0) {
                auto entity = registry.create();
                struct array_frame arr_f = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, env_arr[i][j], terrain_shader);
                registry.emplace<array_frame>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width / 2.0f + j * tile_width, 
                        tile_height / 2.0f + i * tile_height, 0.0f));

                //CHANGE LATER
                registry.emplace<environment>(entity, 1);
                env_cache[j][i] = entity;
            }
        }
    }

    for (size_t i = 0; i < char_arr.size(); i ++) {
        for (size_t j = 0; j < char_arr[i].size(); j++) {
            int& layer = char_arr[i][j];
            if (layer >= 0) {
                auto entity = registry.create();
                struct array_frame_node arr_f = gorge::build_array_frame_node(injector, tile_width, tile_height,
                        tex_name, layer, layer, character_shader);
                registry.emplace<array_frame_node>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width + j * tile_width, 
                        tile_height + i * tile_height, 0.0f));

                //TODO: CHANGE LATER
                registry.emplace<character>(entity, 1);
                char_cache[j][i] = entity;
            }
        }
    }
}

void TileMap2D::move_cusor(entt::registry& registry, unsigned int mouse_x, unsigned int mouse_y) {
    if (mouse_x > 0 && mouse_y > 0 && 
        (mouse_x / tile_width != cursor_x || mouse_y / tile_height != cursor_y)) {
        cursor_x = mouse_x / tile_width;
        cursor_y = mouse_y / tile_height;
        registry.replace<position>(cursor, glm::vec3(tile_width / 2.0f + cursor_x * tile_width, 
            tile_height / 2.0f + cursor_y * tile_height, 0.0f));
    }
}

entt::entity TileMap2D::get_char_on_cursor(entt::registry& registry) {

    if (cursor_x > 0 && char_cache[0].size() > cursor_x && 
        cursor_y > 0 && char_cache.size() > cursor_y) {
            entt::entity value = char_cache[cursor_y][cursor_x];

            if (value != entt::null) {
                last_char_id = registry.get<character>(value).type_id;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

entt::entity TileMap2D::get_env_on_cursor(entt::registry& registry) {
    if (cursor_x > 0 && env_cache[0].size() > cursor_x && 
        cursor_y > 0 && env_cache.size() > cursor_y) {

            entt::entity value = env_cache[cursor_y][cursor_x];

            if (value != entt::null) {
                last_env_id = registry.get<environment>(value).type_id;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

entt::entity TileMap2D::get_terrain_on_cursor(entt::registry& registry) {
    if (cursor_x > 0 && terrain_cache[0].size() > cursor_x && 
        cursor_y > 0 && terrain_cache.size() > cursor_y) {
        
            entt::entity value = terrain_cache[cursor_y][cursor_x];

            if (value != entt::null) {
                last_terrain_id = registry.get<terrain>(value).type_id;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}