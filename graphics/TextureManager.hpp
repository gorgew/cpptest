#pragma once

#include <stb_image.h>

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief A class that manages and creates GL Texture Objects
*/
class TextureManager {

    std::unordered_map<std::string, GLuint> name_to_index_map;
    std::vector<int> heights;
    std::vector<int> widths;
    std::vector<int> channels;
    unsigned char *data;

    public:
        ~TextureManager();
        /**
         * @brief Load an array texture PNG from left to right and top to bottom
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
        void add_texture(const std::string name, const char* filepath);

        /**
         * @brief Gets the id of a texture object mapped to the given name
         * @param name Name of texture object
         * @return id of texture object. 0 if name not found
        */
        GLuint get_id(const std::string name);

};