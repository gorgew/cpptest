#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <Injector.hpp>
#include <memory>

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

/**
 * @brief Component for animated graphic
*/
struct array_frame_node : array_frame{

    struct array_frame_node* next;
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

    struct array_frame_node build_array_frame_node(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int start_layer, unsigned int end_layer, std::string program_name);
}