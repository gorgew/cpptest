#pragma once

#include "GraphicsComponents.hpp"

#include <glm/glm.hpp>

struct char_frame_data : frame {
    
    unsigned char character;
    glm::fvec3 color;
    int width;
    int height;
    int bearing_x;
    int bearing_y;
    unsigned int advance;
};

struct string_component {

    std::string text;
    glm::fvec3 position;
};