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

    entt::entity cursor;
    int cursor_id = 4;

    unsigned int cursor_x = 0, cursor_y = 0; //cursor location in tilemap

    std::vector<std::vector<entt::entity>> env_cache;
    std::vector<std::vector<entt::entity>> char_cache; 
    std::vector<std::vector<entt::entity>> terrain_cache;

    std::vector<std::vector<std::string>> terrain_tile_data;
    robin_hood::unordered_map<std::string, tile_data_t> tile_data;
    robin_hood::unordered_set<std::string> tilesets;
    
    void place_tiles(std::string name, sol::state& lua, entt::registry& registry);
    void place_environment(std::string name, sol::state& lua, entt::registry& registry);
    void place_characters(std::string name, sol::state& lua, entt::registry& registry);

    public:
        std::string last_env;
        std::string last_char;
        std::string last_terrain;

        TileMap2D() = default;
        TileMap2D(std::shared_ptr<Injector>, std::string tex_name, std::string terrain_shader, 
                std::string character_shader, entt::registry& registry);

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

        typedef std::optional<glm::ivec2> coord_option;
        struct range {
            int length;
            glm::ivec2 center;
            std::vector<std::vector<coord_option>> in_range; //Holds parent node, coords are relative to center

            bool in_bounds(glm::ivec2 coord);
            std::vector<glm::ivec2> get_path_from_center(int x, int y);
            glm::ivec2 local_to_array_coords(glm::ivec2 coord);

            void print();
        };

        //Set an anchor at current location
        void enable_anchor_mode();

        //Draw a path from anchor to cursor location
        void enable_draw_path_to_cursor();

        void enable_show_range();

        //Get coordinates in range from entity at (x, y) with magnitude 
        range get_range_no_collision(int x, int y, int magnitude);

        //Get coordinates in range from entity at (x, y) with magnitude with colliding against 
        range get_range_collision(int x, int y, int magnitude);

};      