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
};