#include "KeyEventSystem.hpp"

#include <string>
#include <iostream>

KeyAlreadyBoundError::KeyAlreadyBoundError(SDL_Keycode key_code) {
    this->key_code = key_code;
}


// const char* KeyAlreadyBoundError::what() const throw (){
//     std::string key_name { SDL_GetKeyName(key_code) };
//     std::string what_str = "KeyAlreadyBoundError: " + key_name + " is already bound";
//     return what_str.c_str();
// }

void KeyEventSystem::check_key_bound(SDL_Keycode key_code) {
    if (press_keydown_handlers.contains(key_code)) {
        throw new KeyAlreadyBoundError(key_code);
    } else if (held_keydown_handlers.contains(key_code)) {
        throw new KeyAlreadyBoundError(key_code);
    }
}

void KeyEventSystem::add_keydown_handler(SDL_Keycode key_code, 
        const std::function<void(entt::registry&)> keydown_handler) {
            
    mapped_keys.insert(key_code);
    //check_key_bound(key_code);
    press_keydown_handlers.insert({key_code, keydown_handler});
}

void KeyEventSystem::add_held_key_handler(SDL_Keycode key_code, 
        std::function<void(entt::registry&)> keydown_handler, std::function<void(entt::registry&)> keyup_handler) {
    
    //check_key_bound(key_code);
    mapped_keys.insert(key_code);

    held_keydown_handlers.insert({key_code, keydown_handler});
    keyup_handlers.insert({key_code, keyup_handler});
}

void KeyEventSystem::handle_event(entt::registry& registry, SDL_Event e) {

    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        auto key_code = e.key.keysym.sym;

        if (mapped_keys.contains(key_code)) {
            if (e.type == SDL_KEYDOWN) {

                if (e.key.repeat == 0) {
                    held_keys.insert(key_code);
                    
                    if (press_keydown_handlers.contains(key_code)) {
                        press_keydown_handlers[key_code](registry);
                    }
                }
            }
            else if (e.type == SDL_KEYUP) {
                if (held_keys.contains(key_code)) {
                    held_keys.erase(key_code);

                    if (keyup_handlers.contains(key_code)) {
                        keyup_handlers[key_code](registry);
                    }
                }
            }
        }
    }
}

void KeyEventSystem::execute_holds(entt::registry& registry) {
    for (auto k : held_keys) {
        if (held_keydown_handlers.contains(k)) {
            held_keydown_handlers[k](registry);
        }
    }
}

void KeyEventSystem::add_key_combo_handler(std::unordered_set<SDL_Keycode> &arr, 
        const std::function<void(entt::registry&)> combo_handler) {

}
