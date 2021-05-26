#include "FrameBufferObject.hpp"
#include <stdexcept>
#include <iostream>

FrameBufferObject::FrameBufferObject() {

    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FrameBufferObject::addTexture2D(int width, int height) {

    /*
    texIDs.resize(texIDs.size() + 1);

    auto lastElement = texIDs.size() - 1;


    glGenTextures(1, &texIDs[lastElement]);
    */
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
            GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    /*
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + lastElement, 
            GL_TEXTURE_2D, texIDs[lastElement], 0);
    */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_TEXTURE_2D, texID, 0);
}

/*
void FrameBufferObject::addDepthStencilRenderBuffer() {

    glGenRenderbuffers(1, &rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Config::width, 
            Config::height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
            GL_RENDERBUFFER, rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
*/

void FrameBufferObject::activate() {

    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FrameBufferObject::checkCompiled() {
    
    activate();
    std::cout<<"fboID: "<<fboID << "\n";
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        throw std::invalid_argument("FrameBufferOBject::checkCompiled() : "
                "Incomplete FBO");
    }
}

GLuint FrameBufferObject::getTexID(int pos) {

    return texIDs[pos];
}

FrameBufferObject::~FrameBufferObject() {
    std::cout<<"FBO DYING \n";
}