#include "AudioComponent.hpp"
#include "AudioSystem.hpp"
#include <fmt/format.h>
#include <cstdlib>
AudioSystem::AudioSystem() {

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

AudioSystem::~AudioSystem() {

    for (auto [k, v] : name_to_effect_map) {
        Mix_FreeChunk(v);
    }
    for (auto [k, v] : name_to_music_map) {
        Mix_FreeMusic(v);
    }
    Mix_CloseAudio();
}

void AudioSystem::add_music(std::string name, const char* filepath) {

    auto music_ptr = Mix_LoadMUS(filepath);
    if (!music_ptr) {
        fmt::print("AudioSystem: Failed to load {} at {}\n", name, filepath);
        std::exit(1);
    }
    name_to_music_map[name] = music_ptr;
    
}

void AudioSystem::add_effect(std::string name, const char* filepath) {

    auto effect_ptr = Mix_LoadWAV(filepath);
    if (!effect_ptr) {
        fmt::print("AudioSystem: Failed to load {} at {}\n", name, filepath);
        std::exit(1);
    }
    name_to_effect_map[name] = effect_ptr;
    
}

void AudioSystem::handle_request(entt::registry& registry) {

    auto view = registry.view<audio_request>();
    for (auto [entity, request] : view.each()) {

        auto& effect_value = name_to_effect_map[request.name];
        if (effect_value) {
            Mix_PlayChannel(-1, effect_value, 0);
        }
        auto& music_value = name_to_music_map[request.name];
        if (music_value) {
            Mix_PlayMusic(music_value, -1);
        }
        registry.destroy(entity);
    }
}