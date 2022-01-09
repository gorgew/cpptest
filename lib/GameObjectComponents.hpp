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
    unsigned int type_id;
};

struct environment {
    unsigned int type_id;
};

struct character {
    unsigned int type_id;
};

struct ui {
    bool is_ui = 1;
};
