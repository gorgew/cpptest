#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {
    GLint view_uni_loc;
    glm::vec3 pos;
    
    public:
        Camera(GLint view_uni_loc);

};