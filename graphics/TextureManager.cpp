#include "TextureManager.hpp"

#include <fmt/format.h>

#include <stdexcept>
#include <cstdlib>

void TextureManager::add_2d_array_texture(const std::string name, const char* filepath, 
        int width, int height, int layers){
    
    auto &value = name_to_index_map[name];
    if (!value) {

        GLenum format = GL_RGBA;
        GLuint index = name_to_index_map.size();

        heights.emplace_back();
        widths.emplace_back();
        channels.emplace_back();
        value = index;

        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &index);
        glBindTexture(GL_TEXTURE_2D_ARRAY, index);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        data = stbi_load(filepath, &widths[index], &heights[index], &channels[index], 0);

        if (!data) {
            throw new std::invalid_argument("TextureManager:: data loading failure");
        }

        int& data_height = heights[index];
        int& data_width = widths[index];

        if (data_width % width != 0 || data_height % height != 0) {
            throw new std::invalid_argument("TextureManager:: Invalid height/ width");
        }
        //Rows of layers
        int rows = data_height / height;
        //Cols of layers
        int cols = data_width / width;
        //Put the array texture into a vertical strip
        int strip_cols = 4 * width;
        int area = strip_cols * height;
        int strip_rows = height * layers;
        int layer_counter = 0;
        unsigned char *strip;

        if (data_width != width) {
            
            strip = (unsigned char*) std::malloc(sizeof(unsigned char) * area * layers);

            int gather_row = 0;
            int gather_col = 0;
            int row = 0;
            int data_it = 0;
            while (layer_counter != layers) {
                for (int col = 0; col < strip_cols; ++col) {
                    data_it = col + row * data_width * 4 + (cols - gather_col - 1) * strip_cols 
                            + gather_row * area * cols;
                    *(strip + col + strip_cols * row + layer_counter * area) = *(data + data_it);
                }
                ++row;
                if (row % height == 0) {
                    ++layer_counter;
                    row = 0;
                    gather_row = layer_counter / cols;
                    gather_col = layer_counter % cols;
                }
            }
        } else {
            strip = data;
        }


        //Load the vertical strip as a 2d array texture
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width, height, layers, 0, format,
                GL_UNSIGNED_BYTE, NULL);
                
        int ptr_count = layers - 1;
        
        
        for (int i =0 ; i < layers; i++) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                0, 0,
                i, 
                width, height,
                1, 
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                strip + area * ptr_count);

            --ptr_count;
        }
        
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        fmt::print("Loaded {} at {}\n", name, filepath);

        stbi_image_free(data);

        if (data_width != width) {
            free(strip);
        }
    }

}

void TextureManager::add_texture(const std::string name, const char* filepath) {

    auto &value = name_to_index_map[name];
    if (!value) {
        GLenum format = GL_RGBA;
        GLuint index = name_to_index_map.size() + 1;

        heights.emplace_back();
        widths.emplace_back();
        channels.emplace_back();
        name_to_index_map.insert({name, index});

        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &index);
        glBindTexture(GL_TEXTURE_2D, index);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        data = stbi_load(filepath, &widths[index], &heights[index], &channels[index], 0);

        if (data) {

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, widths[index], heights[index], 0, format, 
                    GL_UNSIGNED_BYTE, data);
                    
            glGenerateMipmap(GL_TEXTURE_2D);
            fmt::print("Loaded {} at {}\n", name, filepath);
        } 
        else {
                    
            fmt::print("Failed to load {} at {}\n", name, filepath);
        }

        stbi_image_free(data);

    } 
}

GLuint TextureManager::get_id(const std::string name) {
    auto &value = name_to_index_map[name];
    if (value) {
        return value;
    }
    else {
        fmt::print("Texture not found: {}\n", name);
        std::exit(1);
    }
}

TextureManager::~TextureManager() {
    for (auto [k,v] : name_to_index_map) {
        glDeleteTextures(1, &v);
    }
}