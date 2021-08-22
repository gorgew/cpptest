#pragma once

#include <entt/entity/registry.hpp>
#include "Injector.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Initialize tile entities? Environment only?
*/
class TileMap2D {

    std::shared_ptr<Injector> injector;
    //Some sort of mapping from string to tile type?
    std::unordered_map<std::string, int> name_to_tile_type_map;

    //Entries are layer number, negatives are ignored
    std::vector<std::vector<int>> terrain_array;
    std::vector<std::vector<int>> environment_array;
    std::vector<std::vector<int>> character_array;

    std::string tex_name;
    std::string shader_name;
    float tile_width;
    float tile_height;

    public:
        TileMap2D() = default;
        TileMap2D(std::shared_ptr<Injector>, std::string, std::string);
        void add_tiles(entt::registry&, std::vector<std::vector<int>>,
                std::vector<std::vector<int>>, std::vector<std::vector<int>>);
        /**
         * for moving in grid
         * Add to position vector
        */

        //For now top plane only?

        //Flat tile = perpindiclar ground, not tilted / wall
        
};