#include "GraphicsComponents.hpp"
#include <fmt/format.h>
#include <cstdlib>

struct frame gorge::build_frame(std::shared_ptr<Injector> injector, float width, float height, 
        std::string tex_array_name, unsigned int layer, std::string program_name) {
    
    //Constructing the vertex array
    std::string va_name = std::to_string((int) width) + std::to_string((int) height) + "rect";
    fmt::print("va_name {}\n", va_name);
    injector->vert_man.add_rect(va_name, width, height);

    return {
        injector->vert_man.get_array_id(va_name), 
        20,
        GL_TRIANGLE_FAN,
        injector->tex_man.get_id(tex_array_name),
        layer,
        injector->shader_man.get_program_id(program_name)
    };
}

struct frame_node gorge::build_frame_node(std::shared_ptr<Injector> injector, float width, float height, 
        std::string tex_array_name, unsigned int start_layer, unsigned int end_layer, std::string program_name) {
    
    int frame_count = end_layer - start_layer + 1;
    frame_node* frame_array = (frame_node*) std::malloc(sizeof(frame_node) * frame_count);
    frame_node* head = frame_array;
    /*
    frame_node first_frame = { build_frame(injector, width, height, tex_array_name, 
                start_layer, program_name), NULL};
    frame_node* head = &first_frame;
    frame_node* next_frame;
    for (int i = 1; i < frame_count; ++i) {
        next_frame = (frame_node*) std::malloc(sizeof(frame_node));
        fmt::print("next layer {}\n", start_layer + i);
        *next_frame = {build_frame(injector, width, height, tex_array_name, 
                start_layer + i, program_name), NULL};
        head->next = next_frame;
        head = next_frame;
    }
    head->next = &first_frame;
    */
   for (int i = 0; i < frame_count - 1; i ++) {
       *(frame_array + i) = { build_frame(injector, width, height, tex_array_name, 
                start_layer + i, program_name), frame_array + i + 1};
   }
   *(frame_array + frame_count - 1) = { build_frame(injector, width, height, tex_array_name, 
                start_layer + frame_count - 1, program_name), frame_array};
   
    //Iterate through all but the last frame
    /*
    for (int i = 0; i < frame_count - 1; ++i) {
        fmt::print("layer {}", start_layer + i);
        frame_array[i] = { build_frame(injector, width, height, tex_array_name, 
                start_layer + i, program_name), frame_array};
    }
    frame_array[frame_count] = {build_frame(injector, width, height, tex_array_name, 
                end_layer, program_name), frame_array};
    */
    //return first_frame;
    return *frame_array;
}