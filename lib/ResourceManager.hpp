#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#include <nuklear/nuklear.h>
#include <robin_hood/robin_hood.h>
#include <ScriptEngine.hpp>
#include <Injector.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <GraphicsComponents.hpp>
#include <GameObjectComponents.hpp>
#include <glm/glm.hpp>

class ResourceManager {

    robin_hood::unordered_map<std::string, struct nk_image> profile_pics;

    robin_hood::unordered_map<std::string, std::vector<array_frame>> frame_vectors;
    robin_hood::unordered_map<std::string, std::vector<int>> timing_vectors;
    robin_hood::unordered_map<std::string, glm::vec3> offsets;
    robin_hood::unordered_map<std::string, bool> loops;

    std::shared_ptr<Injector> injector;

    int char_sprite_dim = 32;
    int char_sheet_count = 2;

    public:

        ResourceManager(std::shared_ptr<Injector> injector);
        void load_character_resources(sol::state& lua);

        //For the nuklear ui
        struct nk_image get_profile_pic(std::string name);

        void add_animation(std::string name, std::string filepath, int tile_width, int tile_height, int num_layers,
            int width, int height,
            std::string program_name, bool loop,
            std::vector<int> frames, std::vector<int> timings, glm::vec3 offset = glm::vec3(0.0f));

        void set_animation(entt::registry& registry, entt::entity entity, std::string name);
};