#pragma once

#include "FontComponents.hpp"
#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <Locator.hpp>


#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <tuple>
#include <utility>
#include <memory>
#include <string>

struct font_data {
    unsigned int max_height;
};

class FontBuilder {
    GLuint char_vert_id;
    GLuint program_id;
    FT_Library ft_lib;
    std::map<std::tuple<std::string, unsigned int, unsigned char>, char_frame_data> font_char_to_data_map;
    std::map<std::pair<std::string, unsigned int>, font_data> font_to_data_map;
    //robin_hood::unordered_node_map<std::pair<std::string, unsigned int>, TextureManager> font_height_pair_to_tex_man_map;
    Locator locator;
    public:
        FontBuilder();

        void add_font(std::string font_name, const char* file_path, unsigned int height);

        char_frame_data get_char(std::string font_name, unsigned int height, unsigned char character);

        void add_string(entt::registry& registry, std::string str, std::string font, unsigned int height, glm::vec3 pos, glm::vec3 color);

        font_data get_font_data(std::string font_name, unsigned int height);

        void build_string_frame(entt::registry& registry, std::string text);

        ~FontBuilder();
};