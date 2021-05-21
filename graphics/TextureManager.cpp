#include "TextureManager.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

void TextureManager::add_2d_array_texture(const std::string name, const char* filepath, 
        int width, int height, int layers){
    
    GLenum format = GL_RGBA;
    size_t index = tex_ids.size();

    std::cout << "index: " << index << '\n';

    tex_ids.emplace_back();
    heights.emplace_back();
    widths.emplace_back();
    channels.emplace_back();
    name_to_index_map.insert({name, index});

    GLuint& tex_id = tex_ids[index];

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);

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

    std::cout << "data_height / data_width " << data_height << " " << data_width << "\n";

    if (data_width % width != 0 || data_height % height != 0) {
        throw new std::invalid_argument("TextureManager:: Invalid height/ width");
    }

    //Rows of layers
    int rows = data_height / height;
    //Cols of layers
    int cols = data_width / width;

    std::cout << "rows / cols " << rows << " " << cols << "\n";

    //Put the array texture into a vertical strip

    int strip_cols = 4 * width;
    int area = strip_cols * height;
    int strip_rows = height * layers;
    int layer_counter = 0;
    unsigned char *strip;
    bool debug = true;
    if (debug) {
    //if (data_width != width) {
        
        strip = (unsigned char*) std::malloc(sizeof(unsigned char) * area * layers);

        int gather_row = 0;
        int gather_col = 0;
        int col = 0;
        int row = 0;
        int data_it = 0;
        while (layer_counter != layers) {
            
            for (col = 0; col < strip_cols; ++col) {
                data_it = col + row * data_width * 4 + (cols - gather_col - 1) * strip_cols + gather_row * area * cols;
                //data_it = gather_col * strip_cols + (gather_row * area * cols) + col + strip_cols * row ;
                *(strip + col + strip_cols * row + layer_counter * area) = *(data + data_it);
                std::cout << "strip: " << col + strip_cols * row + layer_counter * area<< " data: " 
                    << data_it << "\n";
                std::cout << "row area: " << (gather_row * area * cols) << "\n";    
                /* 
                *(strip) = *(data); 
                /**(strip + 1) = *(data + 1);
                *(strip + 2) = *(data + 2);
                *(strip + 3) = *(data + 3);
                */
            }
            ++row;
            if (row % height == 0) {
                ++layer_counter;
                row = 0;
            }
            std::cout << "height: " << height << "col: " << col << " row: " << row << " division: " << layer_counter / cols << "\n";
            gather_row = layer_counter / cols;
            gather_col = layer_counter % cols;

            std::cout << "layercounter: " << layer_counter << " gather_row: " << gather_row << " gather_col: " << gather_col << "\n"; 
        }
        //strip = data;
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
    std::cout << "Loaded " << name << " at " << filepath << "\n";

    stbi_image_free(data);

    if (debug) {
    //if (data_width != width) {
        free(strip);
    }

}

void TextureManager::add_texture(const char* filepath, const std::string name) {

    GLenum format = GL_RGBA;
    size_t index = tex_ids.size();

    std::cout << "index: " << index << '\n';

    tex_ids.emplace_back();
    heights.emplace_back();
    widths.emplace_back();
    channels.emplace_back();
    name_to_index_map.insert({name, index});

    GLuint& tex_id = tex_ids[index];

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load(filepath, &widths[index], &heights[index], &channels[index], 0);

    std::cout << "width and height: " << widths[index]  << " " << heights[index] << "\n";
    if (data) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, widths[index], heights[index], 0, format, 
                GL_UNSIGNED_BYTE, data);
                
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Loaded " << name << " at " << filepath << "\n";
    } 
    else {
                
        std::cout << "Failed to load " << name << "at " << filepath << "\n";
    }

    stbi_image_free(data);
}

int TextureManager::get_height(const std::string name) {
    if (name_to_index_map.contains(name)) {
        return heights[name_to_index_map[name]];
    }
    else {
        throw new std::out_of_range("Texture " + name + " not found");
    }
}

int TextureManager::get_width(const std::string name) {
    if (name_to_index_map.contains(name)) {
        return widths[name_to_index_map[name]];
    }
    else {
        throw new std::out_of_range("Texture " + name + " not found");
    }
}

int TextureManager::get_channels(const std::string name) {
    if (name_to_index_map.contains(name)) {
        return channels[name_to_index_map[name]];
    }
    else {
        throw new std::out_of_range("Texture " + name + " not found");
    }
}

GLuint TextureManager::get_id(const std::string name) {
    if (name_to_index_map.contains(name)) {
        return tex_ids[name_to_index_map[name]];
    }
    else {
        throw new std::out_of_range("Texture " + name + " not found");
    }
}