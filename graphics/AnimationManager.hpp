#pragma once
#include "GraphicsComponents.hpp"
#include <Injector.hpp>
#include <entt/entt.hpp>

#include <memory>
#include <unordered_map>

/**
 * Resource manager for animations
*/
class AnimationManager {
    std::unordered_map<std::string, std::vector<array_frame>> frame_vectors;
    std::unordered_map<std::string, std::vector<int>> timing_vectors;
    std::unordered_map<std::string, glm::vec3> offsets;
    std::unordered_map<std::string, bool> loops;

    std::shared_ptr<Injector> injector;

    public:
        AnimationManager(std::shared_ptr<Injector>);
        //void set_injector(std::shared_ptr<Injector>);
        
        /**
         * @param frames Indices of frames in sheet
         * @param 
        */
        void add_animation(std::string name, std::string filepath, int width, int height, int num_layers,
            std::string program_name, bool loop,
            std::vector<int> frames, std::vector<int> timings, glm::vec3 offset = glm::vec3(0.0f));

        void set_animation(entt::registry& registry, entt::entity entity, std::string name);
};