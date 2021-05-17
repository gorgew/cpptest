#pragma once

extern "C" {
    #include <SDL.h>
}

#include <vector>
#include <functional>

class MouseEventHandler {

    

    std::vector<std::function<bool(SDL_Event)>> left_handlers;
    std::vector<std::function<bool(SDL_Event)>> right_handlers;
    std::vector<std::function<bool(SDL_Event)>> middle_handlers;
    
    public:
        void handle(SDL_Event e);

        void add_left_handler(std::function<bool(SDL_Event)> left_handler);
        void add_right_handler(std::function<bool(SDL_Event)> right_handler);
        void add_middle_handler(std::function<bool(SDL_Event)> middle_handler);
};