#pragma once

extern "C" {
    #include <glad/glad.h>
}

#include <vector>

class FrameBufferObject{

    
    GLuint rboID;
    std::vector<GLuint> texIDs;
    

    public:
        GLuint fboID;
        GLuint texID;
        
        FrameBufferObject();

        ~FrameBufferObject();

        void addTexture2D(int width, int height);

        void addDepthStencilRenderBuffer(int width, int height);

        void activate();

        void close();

        void checkCompiled();

        GLuint getTexID(int pos);

};