#include "ResourceManager.hpp"
#include <fmt/printf.h>
#include <numeric>

void ResourceManager::load_character_resources() {

    auto& lua = locator.get_scripts()->lua;
    sol::table characters = lua["Characters"];
    
    for (const auto& pair : characters) {
        std::string char_name = (pair.first).as<std::string>();
        std::string pic_name = char_name + "_profile";

        fmt::print("Point 4\n");
        textures->add_texture(pic_name,
            "resources/sprites/" + pic_name + ".png");
        fmt::print("Point 5\n");
        profile_pics[char_name] = nk_image_id(static_cast<int>(textures->get_id(pic_name)));
        
        std::string fpath = characters[char_name]["spritesheet"]["path"];
        int width = characters[char_name]["spritesheet"]["width"];
        int height = characters[char_name]["spritesheet"]["height"];
        int sheet_size = characters[char_name]["spritesheet"]["count"];
        textures->add_2d_array_texture(fpath, fpath, width,
            height, sheet_size);

        int world_x = characters[char_name]["world_x"];
        int world_y = characters[char_name]["world_y"];

        sol::table animations = characters[char_name]["spritesheet"]["animations"];
        
        for (const auto& animation_pair : animations) {
            std::string anim_name = char_name + "." + (animation_pair.first).as<std::string>();
            sol::table anim_data = (animation_pair.second).as<sol::table>();
            int start = anim_data[1];
            int end = anim_data[2];
            std::vector<int> frames(end - start + 1);
            std::iota(frames.begin(), frames.end(), start);
            sol::table timing_table = anim_data[3];
            int timing_size = timing_table.size();
            std::vector<int> timings(timing_size);
            
            for (int i = 1; i <= timing_size; i++) {
                timings[i-1] = timing_table[i];
            }
            add_animation(anim_name, fpath, world_x, world_y, "billboard", anim_data["loop"], frames, timings);
        }
    }
    
}

void ResourceManager::load_environment_resources() {
    auto& lua = locator.get_scripts()->lua;
    sol::table env_objs = lua["EnvObjs"];
    
    for (const auto& pair : env_objs) {
        std::string env_name = (pair.first).as<std::string>();
        int sheet_x = env_objs[env_name]["spritesheet"]["width"];
        int sheet_y = env_objs[env_name]["spritesheet"]["height"];
        int sheet_size = env_objs[env_name]["spritesheet"]["count"];

        std::string fpath = env_objs[env_name]["spritesheet"]["path"];
        textures->add_2d_array_texture(fpath, fpath, 
            sheet_x, 
            sheet_y, 
            sheet_size);
    }
    
}

void ResourceManager::load_resources() {
    load_character_resources();
    load_environment_resources();
}

struct nk_image ResourceManager::get_profile_pic(std::string name) {

    if (profile_pics.contains(name)) {
        return profile_pics[name];
    }
    else {
        fmt::print("Couldn't find {}'s profile pic\n", name);
        return profile_pics["Default"];
    }
}
void ResourceManager::add_animation(std::string name, std::string tex_name,
    int width, int height,
    std::string program_name, bool loop,
    std::vector<int> frames, std::vector<int> timings,
    glm::vec3 offset) {

        if (timings.size() == frames.size()) {
        timing_vectors[name] = timings;
        offsets[name] = offset;
        loops[name] = loop;

        int n = frames.size();
        auto& frame_vector  = frame_vectors[name];
        frame_vector.resize(n);

        for (int i = 0; i < n; i++) {
            frame_vector[i] =  gorge::build_array_frame(width, height, tex_name, 
                frames[i], program_name);
        }
        fmt::print("\nAdded animation {}\n", name);
    }
    else {
        fmt::print("Couldn't load {} because number of frame and timings are not the same\n", name);
    }

}

void ResourceManager::add_animation_texture(std::string name, std::string filepath, 
    int tile_width, int tile_height, int num_layers,
    int width, int height,
    std::string program_name, bool loop,
    std::vector<int> frames, std::vector<int> timings,
    glm::vec3 offset) {

    textures->add_2d_array_texture(name, filepath, tile_width, tile_height, num_layers);
    add_animation(name, name, width, height, program_name, loop, frames, timings, offset);
}

void ResourceManager::set_animation(entt::registry& registry, entt::entity entity, std::string name) {

    animation a;
    a.loop = loops[name];
    a.frames = frame_vectors[name];
    a.timings = timing_vectors[name];
    a.offset = offsets[name];
    a.frame_count = frame_vectors[name].size();

    registry.emplace_or_replace<animation>(entity, a);
}
