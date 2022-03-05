#include "TileMap.hpp"
#include "GraphicsComponents.hpp"
#include "PhysicsComponents.hpp"
#include "GameObjectComponents.hpp"
#include <fmt/printf.h>
#include <stack>

TileMap2D::TileMap2D(std::shared_ptr<Injector> injector, std::string tex_name, 
    std::string terrain_shader, std::string character_shader, entt::registry& registry) {    

    this->injector = injector;
    tile_width = injector->config.tile_width;
    tile_height = injector->config.tile_height;
    this->terrain_shader = terrain_shader;
    this->character_shader = character_shader;

    cursor = registry.create();
    auto cursor_frame = gorge::build_array_frame(injector, tile_width, tile_height,
                        tex_name, cursor_id, terrain_shader);
    registry.emplace<array_frame>(cursor, cursor_frame);
    registry.emplace<ui>(cursor);
    registry.emplace<position>(cursor, glm::vec3(0.0f, 0.0f, 0.0f));

    auto x = get_range_no_collision(1, 1, 2);
    x.print(); 
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
            entt::entity value = char_cache[cursor_x][cursor_y];

            if (value != entt::null) {
                last_char = registry.get<character>(value).name;
                //fmt::print("NONNULL\n");
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
                last_env = registry.get<environment>(value).name;
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
                last_terrain = registry.get<terrain>(value).name;
            }
            
            return value;
    }
    else {
        return entt::null;
    }
}

void TileMap2D::load_tileset(sol::state& lua) {
    sol::table tilesets = lua["Tilesets"];

    for (unsigned int i = 1; i <= tilesets.size(); i++) {
        std::string name = tilesets[i]["name"];
        std::string fpath = tilesets[i]["tileset"];
        int width = tilesets[i]["width"];
        int height = tilesets[i]["height"];
        int count = tilesets[i]["count"];

        injector->tex_man.add_2d_array_texture(name, fpath, width, height, count);

        this->tilesets.insert(name);
        fmt::print("Loaded tileset {} @ \n", name, fpath);
    }
}

void TileMap2D::load_tiles(sol::state& lua) {
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

void TileMap2D::place_characters(std::string name, sol::state& lua, entt::registry& registry) {
    
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
            if (char_map_entry != "_") {
                
                int x_index = x - 1;
                int y_index = height - y - 1;
                spawn_points.emplace_back(std::make_pair(x_index, y_index)); //Use later

                std::string char_name = roster[roster_index];
                
                auto entity = registry.create();

                std::string fpath = characters[char_name]["spritesheet"];
                struct array_frame arr_f = gorge::build_array_frame(injector, 
                    tile_width, tile_height,
                    fpath,
                    0,
                    character_shader);
                registry.emplace<array_frame>(entity, arr_f);
                
                registry.emplace<position>(entity, glm::vec3(
                    tile_height + x_index * tile_height, 
                    tile_width + y_index * tile_width, 0.0f));

                registry.emplace<character>(entity,1,  char_name);
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
/*
void TileMap2D::place_environment(std::string name, sol::state& lua, entt::registry& registry) {
    
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
            if (env_map_entry != "_") {
                
                int x_index = x - 1;
                int y_index = height - y - 1;
                
                auto entity = registry.create();

                std::string fpath = env_objs[env_map_entry]["spritesheet"];
                struct array_frame arr_f = gorge::build_array_frame(injector, 
                    tile_width, tile_height,
                    fpath,
                    0,
                    character_shader);
                registry.emplace<array_frame>(entity, arr_f);
                
                registry.emplace<position>(entity, glm::vec3(
                    tile_height + x_index * tile_height, 
                    tile_width + y_index * tile_width, 0.0f));

                registry.emplace<character>(entity,1,  env_map_entry);
                char_cache[x_index][y_index] = entity;
                
            }
        }
    } 
}
*/
void TileMap2D::reset_map() {
    last_env = "";
    last_terrain = "";
    last_char = "";

    env_cache.clear();
    terrain_cache.clear();
    char_cache.clear();

    terrain_tile_data.clear();
}

void TileMap2D::load_map(std::string name, sol::state& lua, entt::registry& registry) {
    reset_map();

    sol::table map = lua["Maps"][name];
    int width = map["width"];
    int height = map["height"];
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
            fmt::print("TILE {} @ INDEX {}\n", tile_name, index);
            
            auto entity = registry.create();
            
            struct array_frame arr_f = gorge::build_array_frame(injector, tile_width, tile_height,
                    tile_data[tile_name].tileset, tile_data[tile_name].index, terrain_shader);
            
            registry.emplace<array_frame>(entity, arr_f);
            int x_index = x - 1;
            int y_index = height - y - 1;
            registry.emplace<position>(entity, glm::vec3(
                    tile_height / 2.0f + x_index * tile_height, 
                    tile_width / 2.0f + y_index * tile_width, 0.0f));
            
            registry.emplace<terrain>(entity, 1, tile_name);
            fmt::print("X {} Y {}\n", x_index, y_index);
            terrain_cache[x_index][y_index] = entity;
            terrain_tile_data[x_index][y_index] = tile_name;
        }
    }

    place_characters(name, lua, registry);
}

bool TileMap2D::range::in_bounds(glm::ivec2 coord) {
    return coord.x >= 0 && coord.y >= 0 && coord.x < length && coord.y < length;
}

glm::ivec2 TileMap2D::range::local_to_array_coords(glm::ivec2 coord) {
    int center = length / 2;
    fmt::print("length:  {}", length);
    return glm::ivec2(center + coord.x, center + coord.y);
}

void TileMap2D::range::print() {
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

TileMap2D::range TileMap2D::get_range_no_collision(int x, int y, int magnitude) {
    typedef std::pair<glm::ivec2, int> coord_mag_pair;

    coord_mag_pair center = std::make_pair(glm::ivec2(magnitude, magnitude), magnitude + 1);
    std::stack<coord_mag_pair> dfs_stack;

    int len = 2 * magnitude + 1;
    fmt::print("Length {}\n", len);
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
        auto& curr = dfs_stack.top();
        dfs_stack.pop();
        
        if (curr.second != 1) {
            int next_mag = curr.second - 1;

            for (auto offset : adj_check) {
                auto child_coord = curr.first + offset;
                if (res.in_bounds(child_coord)) {
                    auto child_mag = mag_array[child_coord.x][child_coord.y];
                    fmt::print("Curr: ({}, {}) Offset: ({}, {}) ({}, {}) Mag: {} Child_mag: {}\n", 
                        curr.first.x,
                        curr.first.y,
                        offset.x,
                        offset.y,
                        child_coord.x, 
                        child_coord.y,
                        curr.second, 
                        child_mag);
                    
                    if (!res.in_range[child_coord.x][child_coord.y].has_value()) {
                        
                        mag_array[child_coord.x][child_coord.y] = next_mag;
                        res.in_range[child_coord.x][child_coord.y] = curr.first;
                        //dfs_stack.push(std::make_pair(child_coord, next_mag));
                    }
                }
                
                
            }
        }
    }
    return res;
}