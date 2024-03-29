#pragma once

#include <glm/glm.hpp>
#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>
#include <robin_hood/robin_hood.h>
#include "Locator.hpp"
#include <string>
#include <vector>
#include <memory>
#include <GameObjectComponents.hpp>

/**
 * @brief Initialize tile entities? Environment only?
*/
class TileMap {

    public:
        typedef std::optional<glm::ivec2> coord_option;
        struct range {
            int length;
            glm::ivec2 center;
            std::vector<std::vector<coord_option>> in_range; //Holds parent node, coords are relative to center

            bool in_local_bounds(int x, int y);
            std::vector<glm::ivec2> get_path_from_center(int x, int y);
            glm::ivec2 local_to_array_coords(int x, int y);

            void print();
        };
private:
    int width, height;

    Locator locator;

    entt::registry& registry = locator.get_registry();
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

    int neutral_tile_id = 6;
    int enemy_tile_id = 7;
    int ally_tile_id = 6;
    range player_range;
    std::vector<entt::entity> player_range_entities;

    int ur_ld_path = 14;
    int ul_rd_path = 15;
    int dr_lu_path = 16;
    int dl_ru_path = 17;
    int horizontal_path = 8;
    int vertical_path = 9;
    int up_arrow_path = 10;
    int right_arrow_path = 11;
    int left_arrow_path = 12;
    int down_arrow_path = 13;
    
    void get_cursor_path(unsigned int mouse_x, unsigned int mouse_y);
    void store_cursor_path(unsigned int mouse_x, unsigned int mouse_y);
    

    std::vector<entt::entity> path_entities;
    void draw_path();


    std::vector<std::vector<entt::entity>> env_cache;
    std::vector<std::vector<entt::entity>> char_cache; 
    std::vector<std::vector<entt::entity>> terrain_cache;

    std::vector<std::vector<std::string>> terrain_tile_data;
    robin_hood::unordered_map<std::string, tile_data_t> tile_data;
    robin_hood::unordered_set<std::string> tilesets;
    
    entt::entity create_tile( int x, int y, std::string tileset, int tile_index);
    entt::entity create_billboard_tile( int x, int y, int width, int height,
        std::string tileset, int tile_index);
    void place_tiles(std::string name, sol::state& lua);
    void place_environment(std::string name, sol::state& lua);
    void place_characters(std::string name, sol::state& lua);
    
    public:
        std::string last_env;
        std::string last_char;
        int last_char_x;
        int last_char_y;
        std::string last_terrain;

        TileMap() = default;
        TileMap(std::string tex_name, std::string terrain_shader, 
                std::string character_shader);
        
        bool in_bounds(int x, int y);
        /**
         * Moves cursor to mouse coords in world space
         */
        void move_cusor(unsigned int mouse_x, unsigned int mouse_y);

        //Return 0 if cursor not in range, 1 if cursor in range
        bool move_cursor_path(unsigned int mouse_x, unsigned int mouse_y);
        bool cursor_in_range(range& r);
        entt::entity get_char_on_cursor();
        entt::entity get_env_on_cursor();
        entt::entity get_terrain_on_cursor();

        void reset_map();
        void load_map(std::string name, sol::state& lua);

        void load_tileset(sol::state& lua);

        void load_tiles(sol::state& lua);

        //Set an anchor at current location
        void enable_anchor_mode();

        void enable_show_range();

        //Get coor  nates in range from entity at (x, y) with magnitude 
        range get_range_no_collision(int x, int y, int magnitude);

        //Get coordinates in range from entity at (x, y) with magnitude with colliding against 
        range get_range_collision(int x, int y, int magnitude);

        void add_player_range( int x, int y, int magnitude, bool collisions = true, range_t tgt = range_t::neutral);
        void add_player_range_cursor( int magnitude);
        void clear_player_range();

        void clear_path();
        std::vector<std::pair<direction, float>> stored_path;
        //Return true if successful, return false if src or tgt are null or if either are out of bounds
        bool move_character( int src_x, int src_y, int tgt_x, int tgt_y);
        //Return true if sucessful, return false if cursor is out of bounds
        bool move_character_selected_cursor();

        bool is_empty_pos(int x, int y);
};