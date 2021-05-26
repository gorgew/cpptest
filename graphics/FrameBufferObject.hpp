#pragma once

extern "C" {
    #include <glad/glad.h>
}

#include <vector>

class FrameBufferObject{

    GLuint fboID;
    GLuint rboID;
    std::vector<GLuint> texIDs;
    

    public:
        GLuint texID;
        
        FrameBufferObject();

        ~FrameBufferObject();

        void addTexture2D(int width, int height);

        void addDepthStencilRenderBuffer();

        void activate();

        void close();

        void checkCompiled();

        GLuint getTexID(int pos);

};