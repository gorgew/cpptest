#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <entt/entity/registry.hpp>
#include <string>
#include <memory>
#include <unordered_map>

class AudioSystem {

    std::unordered_map<std::string, Mix_Music*> name_to_music_map;
    std::unordered_map<std::string, Mix_Chunk*> name_to_effect_map;

    public:
        AudioSystem();

        ~AudioSystem();

        void add_music(std::string name, const char* filepath);
        void add_effect(std::string name, const char* filepath);
        void handle_request(entt::registry& registry);
};