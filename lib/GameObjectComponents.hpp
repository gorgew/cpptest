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

enum class faction {
    player,
    enemy
};

enum class direction {
    up,
    down,
    left,
    right
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
    faction fac;
    direction dir;
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

enum class skill_indicator_t {
    melee,
    path,
    ranged
};

enum class range_t {
    enemy,
    ally,
    neutral
};

