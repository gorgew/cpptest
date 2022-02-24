#pragma once

#include <glm/glm.hpp>
#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>
#include "Injector.hpp"
#include <robin_hood/robin_hood.h>
#include <string>
#include <vector>
#include <memory>
#include <GameObjectComponents.hpp>

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

    std::vector<std::vector<std::string>> terrain_tile_data;
    robin_hood::unordered_map<std::string, tile_data_t> tile_data;
    robin_hood::unordered_set<std::string> tilesets;

    void place_characters(std::string name, sol::state& lua, entt::registry& registry);

    public:
        unsigned int last_env_id;
        unsigned int last_char_id;
        unsigned int last_terrain_id;

        TileMap2D() = default;
        TileMap2D(std::shared_ptr<Injector>, std::string tex_name, std::string terrain_shader, 
                std::string character_shader, entt::registry& registry);
        void add_tiles(entt::registry&, std::vector<std::vector<int>>,
                std::vector<std::vector<int>>, std::vector<std::vector<int>>);
        /**
         * Moves cursor to mouse coords in world space
         */
        void move_cusor(entt::registry&, unsigned int mouse_x, unsigned int mouse_y);

        entt::entity get_char_on_cursor(entt::registry& registry);
        entt::entity get_env_on_cursor(entt::registry& registry);
        entt::entity get_terrain_on_cursor(entt::registry& registry);

        void reset_map();
        void load_map(std::string name, sol::state& lua, entt::registry& registry);

        void load_tileset(sol::state& lua);

        void load_tiles(sol::state& lua);
};      