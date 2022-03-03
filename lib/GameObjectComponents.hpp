#pragma once

#include <string>

struct world_pos {
    unsigned int x;
    unsigned int y;
    unsigned int z;
};

struct old_world_pos {
    unsigned int x;
    unsigned int y;
    unsigned int z;
};

struct terrain {
    unsigned int id;
    std::string name;
};

struct environment {
    unsigned int id;
    std::string name;
};

struct character {
    unsigned int id;
    std::string name;
};

struct ui {
    bool is_ui = 1;
};

struct tile_data_t {
    std::string tileset;
    int index;
    int movement;
    int dodge;
    int heal;
    bool walkable;
    bool flyable;
};