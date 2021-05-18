#include "TextureManager.hpp"
#include <iostream>
#include <stdexcept>

void TextureManager::add_texture(std::string filepath, const std::string name) {

    GLenum format = GL_RGBA;
    size_t index = tex_ids.size();

    std::cout << "index: " << index << '\n';

    tex_ids.emplace_back();
    height.emplace_back();
    width.emplace_back();
    channels.emplace_back();
    name_to_index_map.insert({name, index});

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &tex_ids[index]);
    glBindTexture(GL_TEXTURE_2D, tex_ids[index]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load(filepath.c_str(), &width[index], &height[index], &channels[index], 0);

    if (data) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[index], height[index], 0, format, 
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
        return height[name_to_index_map[name]];
    }
    else {
        throw new std::out_of_range("Texture " + name + " not found");
    }
}

int TextureManager::get_width(const std::string name) {
    if (name_to_index_map.contains(name)) {
        return width[name_to_index_map[name]];
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