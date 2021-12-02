#include "AudioSystem.hpp"
#include <fmt/format.h>
#include <cstdlib>
AudioSystem::AudioSystem() {

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    queued_music.reset();
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

void AudioSystem::enqueue_music(std::string name) {
    queued_music = name;
}

void AudioSystem::enqueue_effect(std::string name) {
    queued_effects.push(name);
}

void AudioSystem::handle_request() {

    while (!queued_effects.empty()) {
        Mix_PlayChannel(-1, name_to_effect_map[queued_effects.front()], 0);
        queued_effects.pop();
    }
    if (queued_music.has_value()) {
        Mix_PlayMusic(name_to_music_map[*queued_music], -1);
        queued_music.reset();
    }
    
}