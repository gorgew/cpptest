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

#include <ScriptEngine.hpp>
#include <Injector.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class ResourceManager {

    std::unordered_map<std::string, struct nk_image> profile_pics;

    public:
        void load_character_resources(sol::state& lua, std::shared_ptr<Injector> injector);

        //For the nuklear ui
        struct nk_image get_profile_pic(std::string name);
};