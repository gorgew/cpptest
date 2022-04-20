#include "Window.hpp"
#include <fmt/core.h>
#include <glad/glad.h>

Window::Window(std::string name, int height, int width) {
    
    this->height = height;
    this->width = width;
    this->name = name;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetSwapInterval(1);
    windowPtr = SDL_CreateWindow(name.c_str(), 100, 100, 
                                        width, height, 
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    glContext = SDL_GL_CreateContext(windowPtr);
    fmt::print("Made window with height {} and with width {}\n", height, width);
    //glViewport(0, 0, 1000, 1000);  
}

void Window::viewport(){

    glViewport(0, 0, width, height);
}

void Window::swap() {
    
    SDL_GL_SwapWindow(windowPtr);
}
void Window::shutdown() {
    
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(windowPtr); 
    SDL_Quit();
    
}

