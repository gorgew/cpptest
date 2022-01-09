#pragma once

#include <glm/glm.hpp>
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

    std::string tex_name;
    std::string terrain_shader;
    std::string character_shader;
    unsigned int tile_width;
    unsigned int tile_height;
    
    int selected_tex;
    glm::vec2 selected;

    int range_tex;

    entt::entity cursor;
    int cursor_id = 4;

    unsigned int cursor_x = 0, cursor_y = 0; //cursor location in tilemap

    std::vector<std::vector<entt::entity>> env_cache;
    std::vector<std::vector<entt::entity>> char_cache; 
    std::vector<std::vector<entt::entity>> terrain_cache;

    public:
        TileMap2D() = default;
        TileMap2D(std::shared_ptr<Injector>, std::string tex_name, std::string terrain_shader, 
                std::string character_shader);
        void add_tiles(entt::registry&, std::vector<std::vector<int>>,
                std::vector<std::vector<int>>, std::vector<std::vector<int>>);
        /**
         * Moves cursor to mouse coords in world space
         */
        void move_cusor(entt::registry&, unsigned int mouse_x, unsigned int mouse_y);

        entt::entity get_char_on_cursor(entt::registry& registry);
        entt::entity get_env_on_cursor(entt::registry& registry);
        entt::entity get_terrain_on_cursor(entt::registry& registry);

        unsigned int last_env_id;
        unsigned int last_char_id;
        unsigned int last_terrain_id;
};      