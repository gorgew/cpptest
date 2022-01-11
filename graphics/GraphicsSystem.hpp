#pragma once

extern "C" {
    #include <glad/glad.h>
}
#include "FontComponents.hpp"
#include "FrameBufferObject.hpp"
#include <Injector.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <entt/entity/registry.hpp>

class GraphicsSystem {

    //Dimensions of fbo and default buffer
    int fbo_width, fbo_height, default_width, default_height;

    std::shared_ptr<Injector> injector;

    std::unique_ptr<FrameBufferObject> offscreenBuffer;

    GLuint buffer_vertices = 0;

    GLuint double_buffer_program = 0;

    GLuint active_program = 0;

    glm::mat4 model;

    void open(GLADloadproc g);

    void draw_frame_component(struct frame f, struct position pos);

    void draw_array_frame_component(struct array_frame f, struct position pos);

    void draw_char_component(struct char_frame_data f, struct position pos);

    void set_program(struct frame f);
    void draw_frame(struct frame f);
    void draw_array_frame(struct frame f);
    public:
        int frame_count = 0;
        int frame_delay; // Frames skipped for animations
        /**
         * @brief Graphics system that draws entities with graphics components.
         *        Also initializes OpenGL.
         * @param injector Dependency injector
         */
        GraphicsSystem(std::shared_ptr<Injector> injector);

        ~GraphicsSystem();

        /**
         * @brief Draw the graphics components of entities
         * @param registry EnTT registry
        */
        void draw(entt::registry& registry, float delta_time);

        void resize(int width, int height);

        void viewport();
};