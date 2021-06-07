#pragma once

#include <Config.hpp>
#include <TextureManager.hpp>
#include <ShaderManager.hpp>
#include <VertexArrayManager.hpp>
#include <KeyEventSystem.hpp>
#include <memory>

/**
 * @brief Depenency injector
 */
class Injector {

    public:
        /**
         * @brief Initailizes shared pointers
         */
        Injector();
        Config config;

        VertexArrayManager vert_man;
        TextureManager tex_man;
        ShaderManager shader_man;

        KeyEventSystem key_event_system;
};