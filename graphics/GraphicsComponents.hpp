#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <Injector.hpp>
#include <memory>

#include <string>
/**
 * @brief Component for non animated graphic
*/
struct frame{
    GLuint vert_arr_id;
    GLuint vert_count;
    GLenum primitive_type;

    GLuint tex_array_id;
    GLuint tex_layer;
    GLuint program_id;
};

/**
 * @brief Component for animated graphic
*/
struct frame_node : frame{

    struct frame_node* next;
};

namespace gorge {

    /**
     * Factory functioh for static_graphic component for a sprite
    */
    struct frame build_frame(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int layer, std::string program_name);

    /**
     * Factory functioh for static_graphic component for a sprite
    */
    struct frame build_frame_flipped(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int layer, std::string program_name);

    struct frame_node build_frame_node(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int start_layer, unsigned int end_layer, std::string program_name);
}