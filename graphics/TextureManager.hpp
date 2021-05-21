#pragma once

#include <stb_image.h>

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>

class TextureManager {

    std::unordered_map<std::string, size_t> name_to_index_map;
    std::vector<GLuint> tex_ids;
    std::vector<GLuint> tex_arr_ids;
    std::vector<int> heights;
    std::vector<int> widths;
    std::vector<int> channels;
    unsigned char *data;

    public:

        /**
         * @brief Load an array texture
         * @param filepath Filepath to texture
         * @param name Name to bind texture to
         * @param width Width in texels
         * @param height Height in texels
         * @param layers Total number of subtextures
         */
        void add_2d_array_texture(const std::string name, const char* filepath, 
                int width, int height, int layers);

        /**
         * @brief Load a single texture and bind a name to it
         * @param filepath filepath to texture
         * @param name name to bind texture to
         */
        void add_texture(const char* filepath, const std::string name);

        void use_texture(const std::string name);

        int get_height(const std::string name);

        int get_width(const std::string name);

        int get_channels(const std::string name);

        GLuint get_id(const std::string name);

        /**
         * @brief Gets the location of a uniform
         * @param tex_name Name of texture
         * @param uniform_name Name of uniform
         */
        GLint& get_uniform_location(const std::string tex_name, const char* uniform_name);

        void check_textures();
};