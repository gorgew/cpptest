#pragma once

extern "C" {
    #include <SDL.h>
}

#include <robin_hood.h>

#include <array>
#include <vector>
#include <functional>
#include <exception>
#include <span>

class KeyAlreadyBoundError : public std::exception {

    SDL_Keycode key_code;

    public: 
        KeyAlreadyBoundError(SDL_Keycode key_code);

        const char* what() const throw();
};

class KeyEventHandler {

    unsigned int held_key_count;

    robin_hood::unordered_set<SDL_Keycode> held_keys;
    robin_hood::unordered_set<SDL_Keycode> mapped_keys;
    robin_hood::unordered_map<SDL_Keycode, const std::function<void()>> press_keydown_handlers;
    robin_hood::unordered_map<SDL_Keycode, const std::function<void()>> held_keydown_handlers;
    robin_hood::unordered_map<SDL_Keycode, const std::function<void()>> keyup_handlers;
    //robin_hood::unordered_map<robin_hood::unordered_set<SDL_Keycode>, const std::function<void()>> combo_handlers;


    void check_key_bound(SDL_Keycode key_code);

    public:

        void handle_event(SDL_Event e);

        void add_keydown_handler(SDL_Keycode key_code, const std::function<void()> keydown_handler);

        void add_held_key_handler(SDL_Keycode key_code, const std::function<void()> keydown_handler, 
                const std::function<void()> keyup_handler);
        void add_key_release_handler(SDL_Keycode, const std::function<void()> keyup_handler);
        void add_key_combo_handler(robin_hood::unordered_set<SDL_Keycode> &arr, 
                const std::function<void()> combo_handler);

};