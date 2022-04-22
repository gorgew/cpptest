#pragma once
#include "Config.hpp"
#include "VertexArrayManager.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include "AudioSystem.hpp"
#include "ScriptEngine.hpp"

class ResourceManager;

class Locator {
    public:
        static Config* get_config() {
            return config;
        }

        static void provide_config(Config* c) {
            config = c;
        }

        static VertexArrayManager* get_vertices() {
            return vertex_service;
        }

        static void provide_vertices(VertexArrayManager* vam) {
            vertex_service = vam;
        }
        static TextureManager* get_textures() {
            return texture_service;
        }

        static void provide_textures(TextureManager* tex) {
            texture_service = tex;
        }

        static ShaderManager* get_shaders() {
            return shader_service;
        }

        static void provide_shaders(ShaderManager* shad) {
            shader_service = shad;
        }

        static AudioSystem* get_audio() {
            return audio_service;
        }

        static void provide_audio(AudioSystem* aud) {
            audio_service = aud;
        }
        
        static ResourceManager* get_resources() {
            return resource_service;
        }

        static void provide_resources(ResourceManager* rm) {
            resource_service = rm;
        }
        

        static ScriptEngine* get_scripts() {
            return script_service;
        }

        static void provide_scripts(ScriptEngine* se) {
            script_service = se;
        }

    private:
        static Config* config;
        static VertexArrayManager* vertex_service;
        static TextureManager* texture_service;
        static ShaderManager* shader_service;
        static AudioSystem* audio_service;
        static ResourceManager* resource_service;
        static ScriptEngine* script_service;
};