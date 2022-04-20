#include "AnimationManager.hpp"
#include <fmt/printf.h>

AnimationManager::AnimationManager() {}

/*
void AnimationManager::set_injector(std::shared_ptr<Injector> i) {
    //this->injector.reset(i);
}
*/

void AnimationManager::add_animation(std::string name, std::string filepath, 
    int width, int height, int num_layers, std::string program_name, bool loop,
    std::vector<int> frames, std::vector<int> timings,
    glm::vec3 offset) {

    if (timings.size() == frames.size()) {
        timing_vectors[name] = timings;
        offsets[name] = offset;
        loops[name] = loop;

        locator.get_textures()->add_2d_array_texture(name, filepath, width, height, num_layers);

        int n = frames.size();
        auto& frame_vector  = frame_vectors[name];
        frame_vector.resize(n);

        for (int i = 0; i < n; i++) {
            frame_vector[i] =  gorge::build_array_frame(width, height, name, 
                frames[i], program_name);
        }
    }
    else {
        fmt::print("Couldn't load {} because number of frame and timings are not the same\n", name);
    }
    
}

void AnimationManager::set_animation(entt::registry& registry, entt::entity entity, std::string name) {

    animation a;
    a.loop = loops[name];
    a.frames = frame_vectors[name];
    a.timings = timing_vectors[name];
    a.offset = offsets[name];
    a.frame_count = a.frames.size();

    registry.emplace_or_replace<animation>(entity, a);
}