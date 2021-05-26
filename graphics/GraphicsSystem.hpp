#pragma once

extern "C" {
    #include <glad/glad.h>
}

#include "FrameBufferObject.hpp"
#include <Injector.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <entt/entity/registry.hpp>

class GraphicsSystem {

    std::shared_ptr<Injector> injector;

    std::unique_ptr<FrameBufferObject> offscreenBuffer;

    GLuint buffer_vertices = 0;

    GLuint double_buffer_program = 0;

    GLuint active_program = 0;

    glm::mat4 model;

    void open(GLADloadproc g);

    void draw_component(struct frame f, struct position pos);

    void free_frame_node(struct frame_node);

    public:

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
        void draw(entt::registry& registry);

};