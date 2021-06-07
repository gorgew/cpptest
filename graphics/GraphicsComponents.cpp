#include "GraphicsComponents.hpp"
#include <fmt/format.h>
#include <dragonbox/dragonbox_to_chars.h>
#include <cstdlib>

struct frame gorge::build_frame(std::shared_ptr<Injector> injector, float width, float height, 
        std::string tex_name, std::string program_name) {
    
    //Constructing the vertex array

    constexpr int buffer_length = 1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>;
    char width_buffer[buffer_length];
    char height_buffer[buffer_length];

    jkj::dragonbox::to_chars(width, width_buffer);
    jkj::dragonbox::to_chars(height, height_buffer);

    std::string width_str {width_buffer};
    std::string height_str {height_buffer};

    std::string va_name = width_str + "-" + height_str + "-rect";
    injector->vert_man.add_rect(va_name, width, height);

    return {
        injector->vert_man.get_array_id(va_name), 
        20,
        GL_TRIANGLE_FAN,
        injector->tex_man.get_id(tex_name),
        //layer,
        injector->shader_man.get_program_id(program_name)
    };
}


struct frame gorge::build_frame_flipped(std::shared_ptr<Injector> injector, float width, float height, 
        std::string tex_name, std::string program_name) {
    
    //Constructing the vertex array

    constexpr int buffer_length = 1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>;
    char width_buffer[buffer_length];
    char height_buffer[buffer_length];

    jkj::dragonbox::to_chars(width, width_buffer);
    jkj::dragonbox::to_chars(height, height_buffer);

    std::string width_str {width_buffer};
    std::string height_str {height_buffer};

    std::string va_name = width_str + "-" + height_str + "-rect-flipped";
    injector->vert_man.add_rect_flipped(va_name, width, height);

    return {
        injector->vert_man.get_array_id(va_name), 
        20,
        GL_TRIANGLE_FAN,
        injector->tex_man.get_id(tex_name),
        injector->shader_man.get_program_id(program_name)
    };
}

struct array_frame gorge::build_array_frame(std::shared_ptr<Injector> injector, float width, float height, 
            std::string tex_array_name, unsigned int layer, std::string program_name) {
    
    return {
        gorge::build_frame(injector, width, height, tex_array_name, program_name),
        layer
    };
}

struct array_frame_node gorge::build_array_frame_node(std::shared_ptr<Injector> injector, float width, float height, 
        std::string tex_array_name, unsigned int start_layer, unsigned int end_layer, std::string program_name) {
    
    int frame_count = end_layer - start_layer + 1;
    array_frame_node* frame_array = (array_frame_node*) std::malloc(sizeof(array_frame_node) * frame_count);
    
    for (int i = 0; i < frame_count - 1; i ++) {
        *(frame_array + i) = { build_array_frame(injector, width, height, tex_array_name, 
                start_layer + i, program_name), frame_array + i + 1};
    }
    *(frame_array + frame_count - 1) = { build_array_frame(injector, width, height, tex_array_name, 
            start_layer + frame_count - 1, program_name), frame_array};
   
    return *frame_array;
}
