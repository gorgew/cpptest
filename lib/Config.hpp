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
        static constexpr bool debug = true;
        static constexpr unsigned int tile_width = 100;
        static constexpr unsigned int tile_height = 100;
        static constexpr float camera_speed = 5.0f;
        static constexpr float FPS = 60.0f;
        float ui_scale;
};