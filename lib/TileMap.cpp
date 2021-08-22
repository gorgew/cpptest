#include "TileMap.hpp"
#include "GraphicsComponents.hpp"
#include "PhysicsComponents.hpp"
#include "GameObjectComponents.hpp"

TileMap2D::TileMap2D(std::shared_ptr<Injector> injector,
        std::string tex_name, std::string shader_name) {    
    this->injector = injector;
    tile_width = injector->config.tile_width;
    tile_height = injector->config.tile_height;
    this->tex_name = tex_name;
    this->shader_name = shader_name;
}

void TileMap2D::add_tiles(entt::registry& registry, std::vector<std::vector<int>> terrain_arr,
                std::vector<std::vector<int>> env_arr, std::vector<std::vector<int>> char_arr) {
    
    terrain_array = terrain_arr;
    environment_array = env_arr;
    character_array = char_arr;
    //Stick in registry

    for (size_t i = 0; i < terrain_arr.size(); i ++) {
        for (size_t j = 0; j < terrain_arr[i].size(); j++) {
            if (terrain_arr[i][j] >= 0) {
                auto entity = registry.create();
                struct array_frame arr_f = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, terrain_arr[i][j], shader_name);
                registry.emplace<array_frame>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width / 2.0f + j * tile_width, 
                        tile_height / 2.0f + i * tile_height, 0.0f));
                registry.emplace<terrain>(entity);
                registry.emplace<world_pos>(entity, j, i, 0);
            }
        }
    }

    for (size_t i = 0; i < env_arr.size(); i ++) {
        for (size_t j = 0; j < env_arr[i].size(); j++) {
            if (env_arr[i][j] >= 0) {
                auto entity = registry.create();
                struct array_frame arr_f = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, env_arr[i][j], shader_name);
                registry.emplace<array_frame>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width / 2.0f + j * tile_width, 
                        tile_height / 2.0f + i * tile_height, 0.0f));
                registry.emplace<environment>(entity);
                registry.emplace<world_pos>(entity, j, i, 0);
            }
        }
    }

    for (size_t i = 0; i < char_arr.size(); i ++) {
        for (size_t j = 0; j < char_arr[i].size(); j++) {
            int& layer = char_arr[i][j];
            if (layer >= 0) {
                auto entity = registry.create();
                struct array_frame_node arr_f = gorge::build_array_frame_node(injector, tile_width, tile_height,
                        tex_name, layer, layer, shader_name);
                registry.emplace<array_frame_node>(entity, arr_f);
                registry.emplace<position>(entity, glm::vec3(tile_width / 2.0f + j * tile_width, 
                        tile_height / 2.0f + i * tile_height, 0.0f));
                registry.emplace<character>(entity, true, layer);
                registry.emplace<world_pos>(entity, j, i, 0);
            }
        }
    }
}