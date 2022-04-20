#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>
/** 
 * @brief Compiles and manages shaders, shader programs
 */
class ShaderManager {
    std::unordered_map<std::string, GLuint> name_to_ubo;
    std::unordered_map<std::string, GLuint> name_to_shader_map;
    std::unordered_map<std::string, GLuint> name_to_program_map;

    bool check_shader(GLuint shader_id);

    bool check_program(GLuint program_id);

    public:
        ~ShaderManager();
        
        void add_shader(std::string name, std::string filepath, GLenum shader_type);

        void add_program(std::string name, std::initializer_list<std::string> programs);

        void delete_shaders();

        void delete_programs();

        void use(std::string name); 

        void add_ubo(std::string name, size_t size, unsigned int binding);
        void bind_ubo(std::string program, std::string block_name, unsigned int binding);
        GLuint get_program_id(std::string name);
        GLuint get_ubo(std::string name);
};