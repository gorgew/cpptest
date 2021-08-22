#pragma once

class Config {

    public:
        Config();
        /*
         * @brief Window width
         */
        int width;
        /*
         * @brief Window height
         */
        int height;
        static const bool debug = true;
        static const int tile_width = 100;
        static const int tile_height = 100;
        static constexpr float camera_speed = 5.0f;
};