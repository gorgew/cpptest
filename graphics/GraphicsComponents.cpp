#include "GraphicsComponents.hpp"
#include <fmt/format.h>
#include <dragonbox/dragonbox_to_chars.h>
#include <cstdlib>

struct frame gorge::build_frame(float width, float height, 
        std::string tex_name, std::string program_name) {
    Locator locator;
    //Constructing the vertex array

    constexpr int buffer_length = 1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>;
    char width_buffer[buffer_length];
    char height_buffer[buffer_length];

    jkj::dragonbox::to_chars(width, width_buffer);
    jkj::dragonbox::to_chars(height, height_buffer);

    std::string width_str {width_buffer};
    std::string height_str {height_buffer};

    std::string va_name = width_str + "-" + height_str + "-rect";
    locator.get_vertices()->add_rect(va_name, width, height);

    return {
        locator.get_vertices()->get_array_id(va_name), 
        4,
        GL_TRIANGLE_FAN,
        locator.get_textures()->get_id(tex_name),
        //layer,
        locator.get_shaders()->get_program_id(program_name)
    };
}


struct frame gorge::build_frame_flipped(float width, float height, 
        std::string tex_name, std::string program_name) {
    
    //Constructing the vertex array
    Locator locator;
    constexpr int buffer_length = 1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>;
    char width_buffer[buffer_length];
    char height_buffer[buffer_length];

    jkj::dragonbox::to_chars(width, width_buffer);
    jkj::dragonbox::to_chars(height, height_buffer);

    std::string width_str {width_buffer};
    std::string height_str {height_buffer};

    std::string va_name = width_str + "-" + height_str + "-rect-flipped";
    locator.get_vertices()->add_rect_flipped(va_name, width, height);

    return {
        locator.get_vertices()->get_array_id(va_name), 
        4,
        GL_TRIANGLE_FAN,
        locator.get_textures()->get_id(tex_name),
        locator.get_shaders()->get_program_id(program_name)
    };
}

struct array_frame gorge::build_array_frame(float width, float height, 
            std::string tex_array_name, unsigned int layer, std::string program_name) {
    
    return {
        gorge::build_frame(width, height, tex_array_name, program_name),
        layer
    };
}
   
