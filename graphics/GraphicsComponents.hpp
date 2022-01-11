#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <Injector.hpp>
#include <memory>
#include <initializer_list>
#include <vector>
#include <string>

struct frame {
    GLuint vert_arr_id;
    GLuint vert_count;
    GLenum primitive_type;

    GLuint tex_id;
    GLuint program_id;
};

/**
 * @brief Component for non animated graphic
*/
struct array_frame : frame{
    GLuint tex_layer;
};

struct animation{
    bool loop;
    glm::vec3 offset;
    int accumulator = 0; //Time passed in current cycle
    int curr = 0;
    std::vector<array_frame> frames;
    std::vector<int> timings;
    unsigned char frame_count;
};

namespace gorge {

    /**
     * Factory function for static_graphic component for a sprite
    */
    struct frame build_frame(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_name, std::string program_name);

    struct frame build_frame_flipped(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_name, std::string program_name);

    /**
     * Factory function for static_graphic component for a sprite
    */
    struct array_frame build_array_frame(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int layer, std::string program_name);

    struct animation build_animation(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, std::string program_name, 
            std::vector<int> frames, std::vector<int> timings, bool loop,
            float offset_x, float offset_y, float offset_z);
}