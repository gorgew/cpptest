#include "TileMap.hpp"
#include "GraphicsComponents.hpp"
#include "PhysicsComponents.hpp"
#include "GameObjectComponents.hpp"
#include "PhysicsSystem.hpp"
#include <fmt/printf.h>
#include <cstdlib>
#include <stack>

TileMap::TileMap(std::string tex_name, std::string terrain_shader, std::string character_shader, entt::registry& registry) {    

    tile_width = locator.get_config()->tile_width;
    tile_height = locator.get_config()->tile_height;
    this->terrain_shader = terrain_shader;
    this->character_shader = character_shader;
    this->tex_name = tex_name;

    cursor = create_tile(registry, 0, 0, tex_name, cursor_id);
    registry.emplace<ui>(cursor);

    auto x = get_range_no_collision(1, 1, 1);
    //x.print(); 
    //add_player_range(registry, 1, 1, 3);
}

bool TileMap::in_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

entt::entity TileMap::create_tile(entt::registry& registry, int x, int y, 
    std::string tileset, int tile_index) {

    auto entity = registry.create();
    struct array_frame arr_f = gorge::build_array_frame(tile_width, tile_height,
            tileset, tile_index, terrain_shader);
    
    registry.emplace<array_frame>(entity, arr_f);

    registry.emplace<position>(entity, glm::vec3(
            tile_height / 2.0f + x * tile_height, 
            tile_width / 2.0f + y * tile_width, 0.0f));
    
    return entity;
}

entt::entity TileMap::create_billboard_tile(entt::registry& registry, int x, int y, int width, int height,
    std::string tileset, int tile_index) {

    auto entity = registry.create();
    struct array_frame arr_f = gorge::build_array_frame(width, height,
            tileset, tile_index, character_shader);
    
    registry.emplace<array_frame>(entity, arr_f);

    registry.emplace<position>(entity, glm::vec3(
            tile_height + x * tile_height, 
            tile_width + y * tile_width, 0.0f));
    
    return entity;
}

bool TileMap::cursor_in_range(range& r) {
    int mag = r.length / 2;
    int range_x = r.center.x + mag - cursor_x;
    int range_y = r.center.y + mag - cursor_y;
    //fmt::print("range_x {} range_y {}\n", range_x, range_y);
    return r.in_local_bounds(range_x, range_y) && r.in_range[range_x][range_y].has_value();
}

void TileMap::move_cusor(entt::registry& registry, unsigned int mouse_x, unsigned int mouse_y) {

    auto next_cursor_x = mouse_x / tile_width;
    auto next_cursor_y = mouse_y / tile_height;
    if ((next_cursor_x != cursor_x || next_cursor_y != cursor_y)
        && in_bounds(next_cursor_x, next_cursor_y)) {
        cursor_x = next_cursor_x;
        cursor_y = next_cursor_y;
        registry.replace<position>(cursor, glm::vec3(tile_width / 2.0f + cursor_x * tile_width, 
            tile_height / 2.0f + cursor_y * tile_height, 0.0f));
    }
}

void TileMap::clear_path(entt::registry& registry) {
    for (const auto& entity : path_entities) {
        registry.destroy(entity);
    }
    path_entities.clear();
}

void TileMap::store_cursor_path(entt::registry& registry, unsigned int mouse_x, unsigned int mouse_y) {
    
    stored_path.clear();
    auto next_cursor_x = mouse_x;
    auto next_cursor_y = mouse_y;
    float distance = 100.0f;

    if (in_bounds(next_cursor_x, next_cursor_y)) {
        
        if (cursor_in_range(player_range)) {
            
            clear_path(registry);
            direction last_direction;
            int mag = player_range.length / 2;
            auto coord = player_range.local_to_array_coords(cursor_x, cursor_y);
            int range_x = coord.x;
            int range_y = coord.y;

            auto next = player_range.in_range[range_x][range_y].value();
            int diff_x = next.x - range_x;
            int diff_y = next.y - range_y;
            if (diff_x == -1) {
                last_direction = direction::left;
                range_x--;
                next_cursor_x++;
            }
            else if (diff_x == 1) {
                last_direction = direction::right;
                range_x++;
                next_cursor_x--;
            }
            else if (diff_y == -1) {
                last_direction = direction::down;
                range_y--;
                next_cursor_y++;
            }
            else if (diff_y == 1) {
                last_direction = direction::up;
                range_y++;
                next_cursor_y--;
            }
            while (range_x != mag || range_y != mag) {
                //fmt::print("asdf\n");
                next = player_range.in_range[range_x][range_y].value();
                diff_x = next.x - range_x;
                diff_y = next.y - range_y;
                
                if (diff_x == -1) {
                    if (last_direction != direction::left) {
                        stored_path.push_back(std::make_pair(last_direction, distance));
                        distance = 0.0f;
                    }
                    last_direction = direction::left;
                    range_x--;
                    next_cursor_x++;
                }
                else if (diff_x == 1) {
                    if (last_direction != direction::right) {
                        stored_path.push_back(std::make_pair(last_direction, distance));
                        distance = 0.0f;
                    }
                    last_direction = direction::right;
                    range_x++;
                    next_cursor_x--;
                }
                else if (diff_y == -1) {
                    if (last_direction != direction::down) {
                        stored_path.push_back(std::make_pair(last_direction, distance));
                        distance = 0.0f;
                    }
                    last_direction = direction::down;
                    range_y--;
                    next_cursor_y++;
                }
                else if (diff_y == 1) {
                    if (last_direction != direction::up) {
                        stored_path.push_back(std::make_pair(last_direction, distance));
                        distance = 0.0f;
                    }
                    last_direction = direction::up;
                    range_y++;
                    next_cursor_y--;
                }
                distance += 100.0f;
            }
            stored_path.push_back(std::make_pair(last_direction, distance));
            for (auto& p: stored_path) {
                //fmt::print("Direction: {} Distance: {}\n", p.first, p.second);
            }
        }
        std::reverse(stored_path.begin(), stored_path.end());
    }
}

bool TileMap::move_cursor_path(entt::registry& registry, unsigned int mouse_x, unsigned int mouse_y) {

    auto next_cursor_x = mouse_x / tile_width;
    auto next_cursor_y = mouse_y / tile_height;
    if ((next_cursor_x != cursor_x || next_cursor_y != cursor_y)
        && in_bounds(next_cursor_x, next_cursor_y)) {
        cursor_x = next_cursor_x;
        cursor_y = next_cursor_y;
        registry.replace<position>(cursor, glm::vec3(tile_width / 2.0f + cursor_x * tile_width, 
            tile_height / 2.0f + cursor_y * tile_height, 0.0f));
        
        if (cursor_in_range(player_range)) {
            clear_path(registry);
            direction last_direction;
            int mag = player_range.length / 2;
            auto coord = player_range.local_to_array_coords(cursor_x, cursor_y);
            int range_x = coord.x;
            int range_y = coord.y;
            //fmt::print("b4 range_x {} range_y {}\n", range_x, range_y);
            auto next = player_range.in_range[range_x][range_y].value();
            int diff_x = next.x - range_x;
            int diff_y = next.y - range_y;
            if (diff_x == -1) {
                path_entities.push_back(create_tile(registry, next_cursor_x, 
                    next_cursor_y, tex_name, left_arrow_path));
                last_direction = direction::left;
                range_x--;
                next_cursor_x++;
            }
            else if (diff_x == 1) {
                path_entities.push_back(create_tile(registry, next_cursor_x, 
                    next_cursor_y, tex_name, right_arrow_path));
                last_direction = direction::right;
                range_x++;
                next_cursor_x--;
            }
            else if (diff_y == -1) {
                path_entities.push_back(create_tile(registry, next_cursor_x, 
                    next_cursor_y, tex_name, down_arrow_path));
                last_direction = direction::down;
                range_y--;
                next_cursor_y++;
            }
            else if (diff_y == 1) {
                path_entities.push_back(create_tile(registry, next_cursor_x, 
                    next_cursor_y, tex_name, up_arrow_path));
                last_direction = direction::up;
                range_y++;
                next_cursor_y--;
            }
            
            while (range_x != mag || range_y != mag) {

                next = player_range.in_range[range_x][range_y].value();
                diff_x = next.x - range_x;
                diff_y = next.y - range_y;
                
                if (diff_x == -1) {
                    if (last_direction == direction::left) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, horizontal_path));
                    }
                    else if (last_direction == direction::up) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, dr_lu_path));
                    }
                    else if (last_direction == direction::down) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, ur_ld_path));
                    }
                    
                    last_direction = direction::left;
                    range_x--;
                    next_cursor_x++;
                }
                else if (diff_x == 1) {
                    if (last_direction == direction::right) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, horizontal_path));
                    }
                    else if (last_direction == direction::up) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, dl_ru_path));
                    }
                    else if (last_direction == direction::down) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, ul_rd_path));
                    }
                    last_direction = direction::right;
                    range_x++;
                    next_cursor_x--;
                }
                else if (diff_y == -1) {
                    if (last_direction == direction::down) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, vertical_path));
                    }
                    else if (last_direction == direction::left) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, dl_ru_path));
                    }
                    else if (last_direction == direction::right) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, dr_lu_path));
                    }
                    last_direction = direction::down;
                    range_y--;
                    next_cursor_y++;
                }
                else if (diff_y == 1) {
                    if (last_direction == direction::up) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, vertical_path));
                    }
                    else if (last_direction == direction::left) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, ul_rd_path));
                    }
                    else if (last_direction == direction::right) {
                        path_entities.push_back(create_tile(registry, next_cursor_x, 
                        next_cursor_y, tex_name, ur_ld_path));
                    }
                    last_direction = direction::up;
                    range_y++;
                    next_cursor_y--;
                }
            }
            return true;
        }
    }
    return false;
}

entt::entity TileMap::get_char_on_cursor(entt::registry& registry) {

    if (cursor_x >= 0 && char_cache[0].size() > cursor_x && 
        cursor_y >= 0 && char_cache.size() > cursor_y) {
            entt::entity value = char_cache[cursor_x][cursor_y];

            if (value != entt::null) {
                last_char = registry.get<character>(value).name;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

entt::entity TileMap::get_env_on_cursor(entt::registry& registry) {
    if (cursor_x >= 0 && env_cache[0].size() > cursor_x && 
        cursor_y >= 0 && env_cache.size() > cursor_y) {

            entt::entity value = env_cache[cursor_y][cursor_x];

            if (value != entt::null) {
                last_env = registry.get<environment>(value).name;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

entt::entity TileMap::get_terrain_on_cursor(entt::registry& registry) {
    if (cursor_x >= 0 && terrain_cache[0].size() > cursor_x && 
        cursor_y >= 0 && terrain_cache.size() > cursor_y) {
        
            entt::entity value = terrain_cache[cursor_y][cursor_x];

            if (value != entt::null) {
                last_terrain = registry.get<terrain>(value).name;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

void TileMap::load_tileset(sol::state& lua) {
    sol::table tilesets = lua["Tilesets"];

    for (unsigned int i = 1; i <= tilesets.size(); i++) {
        std::string name = tilesets[i]["name"];
        std::string fpath = tilesets[i]["tileset"];
        int width = tilesets[i]["width"];
        int height = tilesets[i]["height"];
        int count = tilesets[i]["count"];

        locator.get_textures()->add_2d_array_texture(name, fpath, width, height, count);

        this->tilesets.insert(name);
        fmt::print("Loaded tileset {} @ \n", name, fpath);
    }
}

void TileMap::load_tiles(sol::state& lua) {
    sol::table tiles = lua["Tiles"];

    for (unsigned int i = 1; i <= tiles.size(); i++) {
        std::string name = tiles[i]["name"];
        std::string l_tileset = tiles[i]["tileset"];
        if (!tilesets.contains(l_tileset)) {
            fmt::print("Loading tile {}, tileset {} not found\n", name, l_tileset);
        }
        auto& value = tile_data[name];

        value = {
            .tileset = l_tileset,
            .index = tiles[i]["index"],
            .movement = tiles[i]["movement"],
            .dodge = tiles[i]["dodge"],
            .heal = tiles[i]["heal"],
            .walkable = tiles[i]["walkable"],
            .flyable = tiles[i]["flyable"]
        };

        fmt::print("Loaded tile {} index {}\n", name, (int) tiles[i]["index"]);
    }
}

void TileMap::place_characters(std::string name, sol::state& lua, entt::registry& registry) {
    
    sol::table map = lua["Maps"][name];
    sol::table char_map = map["CharMap"];
    sol::table characters = lua["Characters"];
    sol::table roster = lua["Roster"];
    
    int roster_size = roster.size();
    int roster_index = 1;
    // Gather spawn points
    std::vector<std::pair<int, int>> spawn_points;
    int width = map["width"];
    int height = map["height"];

    char_cache.resize(width);
    for (int i = 0; i < width; i++) {
        char_cache[i].resize(height);
        for (int j = 0; j < height; j++) {
            char_cache[i][j] = entt::null;
        }
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = width - 1; x >= 0; x--) {
            int index = y * width + x;
            std::string char_map_entry = char_map[index];
            if (char_map_entry != "") {
                
                int x_index = x - 1;
                int y_index = height - y - 1;
                spawn_points.emplace_back(std::make_pair(x_index, y_index)); //Use later

                std::string char_name = roster[roster_index];
                
                std::string spritesheet = characters[char_name]["spritesheet"]["path"];
                int sheet_index = 0;
                auto entity = create_billboard_tile(registry, x_index, y_index, 
                    (int) characters[char_name]["world_x"], 
                    (int) characters[char_name]["world_y"],
                    spritesheet, sheet_index);
                registry.emplace<character>(entity, 1, char_name, faction::player, direction::up);
                char_cache[x_index][y_index] = entity;

                if (roster_index < roster_size) {
                    roster_index++;
                }
                else {
                    goto LOOP_EXIT;
                }
                
            }
        }
    } 
    LOOP_EXIT:;
}

void TileMap::place_environment(std::string name, sol::state& lua, entt::registry& registry) {
    
    sol::table map = lua["Maps"][name];
    sol::table env_map = map["EnvMap"];
    sol::table env_objs = lua["EnvObjs"];
    
    int width = map["width"];
    int height = map["height"];
    
    env_cache.resize(width);
    for (int i = 0; i < width; i++) {
        env_cache[i].resize(height);
        for (int j = 0; j < height; j++) {
            env_cache[i][j] = entt::null;
        }
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = width - 1; x >= 0; x--) {
            int index = y * width + x;
            std::string env_map_entry = env_map[index];
            
            if (env_map_entry != "") {
                
                int x_index = x - 1;
                int y_index = height - y - 1;

                std::string spritesheet = env_objs[env_map_entry]["spritesheet"]["path"];
                
                int index = env_objs[env_map_entry]["index_at_hp"][(int) env_objs[env_map_entry]["hp"]];
                int height = env_objs[env_map_entry]["world_x"];
                int width = env_objs[env_map_entry]["world_y"];
                auto entity = create_billboard_tile(registry, x_index, y_index, height, width,
                    spritesheet, index);

                registry.emplace<environment>(entity, 1,  env_map_entry);
                env_cache[x_index][y_index] = entity;
                
            }
        }
        
    } 
    
}

void TileMap::reset_map() {
    last_env = "";
    last_terrain = "";
    last_char = "";

    env_cache.clear();
    terrain_cache.clear();
    char_cache.clear();

    terrain_tile_data.clear();
}

void TileMap::load_map(std::string name, sol::state& lua, entt::registry& registry) {
    reset_map();
    
    sol::table map = lua["Maps"][name];
    width = map["width"];
    height = map["height"];
    sol::table terrain_map = map["TerrainMap"];

    terrain_cache.resize(width);
    terrain_tile_data.resize(width);
    for (int i = 0; i < width; i++) {
        terrain_cache[i].resize(height);
        terrain_tile_data[i].resize(height);
    }
    
    for (int y = height - 1; y >= 0; y--) {
        for (int x = width; x > 0; x--) {
            int index = y * width + x;
            std::string tile_name = terrain_map[index];
            //fmt::print("TILE {} @ INDEX {}\n", tile_name, index);
            
            int x_index = x - 1;
            int y_index = height - y - 1;

            auto entity = create_tile(registry, x_index, y_index, 
                tile_data[tile_name].tileset, tile_data[tile_name].index);
            
            registry.emplace<terrain>(entity, 1, tile_name);
            fmt::print("X {} Y {}\n", x_index, y_index);
            terrain_cache[x_index][y_index] = entity;
            terrain_tile_data[x_index][y_index] = tile_name;
        }
    }
    place_environment(name, lua, registry);
    place_characters(name, lua, registry);
}

bool TileMap::range::in_local_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < length && y < length;
}

glm::ivec2 TileMap::range::local_to_array_coords(int x, int y) {
    
    int mag = length / 2;
    return glm::ivec2(center.x + mag - x, center.y + mag - y);
}

void TileMap::range::print() {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (in_range[i][j].has_value()) {
                fmt::print("({}, {}), ", in_range[i][j].value().x, in_range[i][j].value().y);
            }
            else {
                fmt::print("      , ");
            }
        }
        fmt::print("\n");
    }
}

TileMap::range TileMap::get_range_no_collision(int x, int y, int magnitude) {
    typedef std::pair<glm::ivec2, int> coord_mag_pair;

    coord_mag_pair center = std::make_pair(glm::ivec2(magnitude, magnitude), magnitude + 1);
    std::stack<coord_mag_pair> dfs_stack;

    int len = 2 * magnitude + 1;

    range res = {.length = len, .center = glm::ivec2(x, y)};
    res.in_range.resize(len);
    for (int i = 0; i < len; i++) {
        res.in_range[i].resize(len);
    }

    std::vector<std::vector<int>> mag_array;
    mag_array.resize(len);
    for (int i = 0; i < len; i++) {
        mag_array[i].resize(len);
    }

    std::initializer_list<glm::ivec2> adj_check = {glm::ivec2(1, 0),
        glm::ivec2(-1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(0, -1)};

    dfs_stack.push(center);
    res.in_range[magnitude][magnitude] = glm::ivec2(-1, -1);
    while (!dfs_stack.empty()) {
        auto curr = dfs_stack.top();
        dfs_stack.pop();
        
        if (curr.second != 1) {
            int next_mag = curr.second - 1;

            for (auto offset : adj_check) {
                auto child_coord = curr.first + offset;
                if (res.in_local_bounds(child_coord.x, child_coord.y)) {
                    if (!res.in_range[child_coord.x][child_coord.y].has_value()) {
                        
                        mag_array[child_coord.x][child_coord.y] = next_mag;
                        res.in_range[child_coord.x][child_coord.y] = curr.first;
                        dfs_stack.push(std::make_pair(child_coord, next_mag));
                    }
                }
            }
        }
    }
    return res;
}

TileMap::range TileMap::get_range_collision(int x, int y, int magnitude) {
    typedef std::pair<glm::ivec2, int> coord_mag_pair;

    coord_mag_pair center = std::make_pair(glm::ivec2(magnitude, magnitude), magnitude + 1);
    std::stack<coord_mag_pair> dfs_stack;

    int len = 2 * magnitude + 1;

    range res = {.length = len, .center = glm::ivec2(x, y)};
    res.in_range.resize(len);
    for (int i = 0; i < len; i++) {
        res.in_range[i].resize(len);
    }

    std::vector<std::vector<int>> mag_array;
    mag_array.resize(len);
    for (int i = 0; i < len; i++) {
        mag_array[i].resize(len);
    }

    std::initializer_list<glm::ivec2> adj_check = {glm::ivec2(1, 0),
        glm::ivec2(-1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(0, -1)};

    dfs_stack.push(center);
    res.in_range[magnitude][magnitude] = glm::ivec2(-1, -1);
    while (!dfs_stack.empty()) {
        auto curr = dfs_stack.top();
        dfs_stack.pop();
        
        if (curr.second != 1) {
            int next_mag = curr.second - 1;

            for (auto offset : adj_check) {
                auto child_coord = curr.first + offset;
                if (res.in_local_bounds(child_coord.x, child_coord.y)) {
                    auto child_mag = mag_array[child_coord.x][child_coord.y];
                    
                    auto world_coord = res.local_to_array_coords(child_coord.x, child_coord.y);
                    
                    if (!res.in_range[child_coord.x][child_coord.y].has_value() 
                        && child_mag < next_mag
                        && in_bounds(world_coord.x, world_coord.y)
                        && env_cache[world_coord.x][world_coord.y] == entt::null
                        ) {
                        
                        mag_array[child_coord.x][child_coord.y] = next_mag;
                        res.in_range[child_coord.x][child_coord.y] = curr.first;
                        dfs_stack.push(std::make_pair(child_coord, next_mag));
                    }
                }
            }
        }
    }
    return res;
}

void TileMap::add_player_range(entt::registry& registry, int x, int y, int magnitude) {
    player_range = get_range_collision(x, y, magnitude); 

    for (int m_x = 0; m_x < player_range.length; m_x++) {
        for (int m_y = 0; m_y < player_range.length; m_y++) {
            if (player_range.in_range[m_x][m_y].has_value()) {
                auto map_coord = player_range.local_to_array_coords(m_x, m_y);
                
                auto entity = create_tile(registry, map_coord.x, map_coord.y, tex_name, player_range_id);
                player_range_entities.push_back(entity);
            }
        }
    }
}

void TileMap::clear_player_range(entt::registry& registry) {
    for (const auto& entity : player_range_entities) {
        registry.destroy(entity);
    }
    player_range_entities.clear();
}

void TileMap::add_player_range_cursor(entt::registry& registry, int magnitude) {
    add_player_range(registry, cursor_x, cursor_y, magnitude);
}

bool TileMap::move_character(entt::registry& registry, int src_x, int src_y, int tgt_x, int tgt_y) {
    if (in_bounds(src_x, src_y) && in_bounds(tgt_x, tgt_y) 
            && char_cache[src_x][src_y] != entt::null 
            && char_cache[tgt_x][tgt_y] == entt::null) {
        char_cache[tgt_x][tgt_y] = char_cache[src_x][src_y];
        /*
        registry.replace<position>(char_cache[src_x][src_y], 
            glm::vec3(
            tile_height + tgt_x * tile_height, 
            tile_width + tgt_y * tile_width, 0.0f)
        );
        */
        char_cache[src_x][src_y] = entt::null;
        return true;
    }
    else {
        return false;
    }
}

bool TileMap::move_character_selected_cursor(entt::registry& registry) {
    if (cursor_in_range(player_range)) {
        store_cursor_path(registry, cursor_x, cursor_y);
        return move_character(registry, player_range.center.x, player_range.center.y, cursor_x, cursor_y);
    }
    else {
        return false;
    }
}

bool TileMap::is_empty_pos(int x, int y) {
    return env_cache[x][y] != entt::null && char_cache[x][y] != entt::null;
}