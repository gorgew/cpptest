#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <entt/entity/registry.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>
#include <optional>

class AudioSystem {

    std::unordered_map<std::string, Mix_Music*> name_to_music_map;
    std::unordered_map<std::string, Mix_Chunk*> name_to_effect_map;

    std::queue<std::string> queued_effects;
    std::optional<std::string> queued_music;

    public:
        AudioSystem();

        ~AudioSystem();

        void add_music(std::string name, const char* filepath);
        void add_effect(std::string name, const char* filepath);
        void enqueue_music(std::string name);
        void enqueue_effect(std::string name);
        void handle_request();
};