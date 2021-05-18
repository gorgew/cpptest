#pragma once

extern "C" {

    #include <glad/glad.h>
}

#include <iostream>

enum class ImageFormat {
    PNG,
    JPEG
};

class Texture2D {
    
    private:
        
        unsigned char *data;
        int height;
        int width;
        int channels;
        GLuint textureID;
    
    public:

        Texture2D(const char* file, ImageFormat i);

        GLuint getID();

        int getWidth();

        int getHeight();

        int getChannels();

        void close();
};