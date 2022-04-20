#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <fmt/core.h>

#include <string>

class Window {

    int height;
    int width;
    
    std::string name;
    
    public:
        SDL_GLContext glContext;
        SDL_Window* windowPtr; 
        Window(std::string name, int height, int width);

        void viewport();

        void swap();
        
        void shutdown();
};