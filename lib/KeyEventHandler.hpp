#pragma once

extern "C" {
    #include <SDL2/SDL.h>
}
#include <array>
#include <vector>
#include <functional>
#include <exception>
#include <span>
#include <unordered_map>
#include <unordered_set>

class KeyAlreadyBoundError : public std::exception {

    SDL_Keycode key_code;

    public: 
        KeyAlreadyBoundError(SDL_Keycode key_code);

         char* what()  throw();
};

class KeyEventHandler {

    unsigned int held_key_count;

    std::unordered_set<SDL_Keycode> held_keys;
    std::unordered_set<SDL_Keycode> mapped_keys;
    std::unordered_map<SDL_Keycode, std::function<void()>> press_keydown_handlers;
    std::unordered_map<SDL_Keycode, std::function<void()>> held_keydown_handlers;
    std::unordered_map<SDL_Keycode, std::function<void()>> keyup_handlers;
    //std::unordered_map<std::unordered_set<SDL_Keycode>,  std::function<void()>> combo_handlers;


    void check_key_bound(SDL_Keycode key_code);

    public:

        void handle_event(SDL_Event e);

        void add_keydown_handler(SDL_Keycode key_code, std::function<void()> keydown_handler);

        void add_held_key_handler(SDL_Keycode key_code, std::function<void()> keydown_handler, 
                std::function<void()> keyup_handler);
        void add_key_release_handler(SDL_Keycode, std::function<void()> keyup_handler);
        void add_key_combo_handler(std::unordered_set<SDL_Keycode> &arr, 
            std::function<void()> combo_handler);

};