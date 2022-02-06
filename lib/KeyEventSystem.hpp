#pragma once

extern "C" {
    #include <SDL2/SDL.h>
}
#include <entt/entt.hpp>
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

class KeyEventSystem {
    
    std::unordered_set<SDL_Keycode> held_keys;
    std::unordered_set<SDL_Keycode> mapped_keys;
    std::unordered_map<SDL_Keycode, std::function<void(entt::registry&)>> press_keydown_handlers;
    std::unordered_map<SDL_Keycode, std::function<void(entt::registry&)>> held_keydown_handlers;
    std::unordered_map<SDL_Keycode, std::function<void(entt::registry&)>> keyup_handlers;
    //std::unordered_map<std::unordered_set<SDL_Keycode>,  std::function<void()>> combo_handlers;

    void check_key_bound(SDL_Keycode key_code);

    public:
        /**
         * @brief Handle a SDL_Event and do stuff to entities in a registry
         * @param registry EnTT registry
         * @param e SDL_Event, should be SDL_MOUSEDOWN or SDL_MOUSEUP
        */
        void handle_event(entt::registry& registry, SDL_Event e);
        
        /**
         * @brief Carry out hold key functions on entities in a registry
         * @param registry EnTT registry
        */
        void execute_holds(entt::registry& registry);

        void add_keydown_handler(SDL_Keycode key_code, std::function<void(entt::registry&)> keydown_handler);

        void add_held_key_handler(SDL_Keycode key_code, std::function<void(entt::registry&)> keydown_handler, 
                std::function<void(entt::registry&)> keyup_handler);
        void add_key_release_handler(SDL_Keycode, std::function<void(entt::registry&)> keyup_handler);
        void add_key_combo_handler(std::unordered_set<SDL_Keycode> &arr, 
            std::function<void(entt::registry&)> combo_handler);

};