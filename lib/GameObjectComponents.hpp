#pragma once

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
    bool is_terrain = 1;
};

struct environment {
    bool is_environment = 1;
};

struct character {
    bool is_character = 1;
};
