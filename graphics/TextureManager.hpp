#pragma once

#include <robin_hood.h>

#include <stb_image.h>

#include <glad/glad.h>
#include <string>
#include <vector>

class TextureManager {

    robin_hood::unordered_map<std::string, size_t> name_to_index_map;
    std::vector<GLuint> tex_ids;
    std::vector<int> height;
    std::vector<int> width;
    std::vector<int> channels;
    unsigned char *data;

    public:

        void add_array_texture();

        /**
         * @brief Load a single texture and bind a name to it
         * @param filepath filepath to texture
         * @param name name to bind texture to
         */
        void add_texture(std::string filepath, const std::string name);

        void use_texture(const std::string name);

        int get_height(const std::string name);

        int get_width(const std::string name);

        int get_channels(const std::string name);

        GLuint get_id(const std::string name);

        void check_textures();
};