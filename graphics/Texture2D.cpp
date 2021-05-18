#include "Texture2D.hpp"

extern "C" {

    #include "../stb_image.h"
}

Texture2D::Texture2D(const char* file, ImageFormat i) {

    GLenum format;
    switch (i) {

        case ImageFormat::JPEG :
            format = GL_RGB;
            break;

        case ImageFormat::PNG : 
            format = GL_RGBA;
            break;
    }

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load(file, &width, &height, &channels, 0);

    if (data) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, 
                GL_UNSIGNED_BYTE, data);
                
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "TEXTURE LOADED" << std::endl;
    } 
    else {
                
        std::cerr << "TEXTURE LOADING FAILED" << std::endl;
    }

    stbi_image_free(data);
}

GLuint Texture2D::getID() {
    
    return textureID;
}

int Texture2D::getWidth() {

    return width;
}
int Texture2D::getHeight() {

    return height;
}

int Texture2D::getChannels() {

    return channels;
}

void Texture2D::close() {

    glDeleteTextures(1, &textureID);
}