#pragma once

#include <unordered_map>
#include <string>
#include <vector>

/**
 * @brief Initialize tile entities? Environment only?
*/
class TileMap {

    //Some sort of mapping from string to tile type?
    std::unordered_map<std::string, int> name_to_tile_type_map;

    public:
        //For now top plane only?

        //Flat tile = perpindiclar ground, not tilted / wall
        
};