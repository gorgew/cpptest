#include "SDL.h"
#include <iostream>
#include <sol/sol.hpp>
//#include <lua.h>

int main(void) {
    std::cout<<"Hello world"<<"\n";

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    if (window == NULL) {
    // In the case that the window could not be made...
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
}

// A basic main loop to prevent blocking
bool is_running = true;
SDL_Event event;
while (is_running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running = false;
        }
    }
    SDL_Delay(16);
}

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}