#include <ShaderManager.hpp>

#include <fmt/format.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

void ShaderManager::add_shader(std::string name, std::string filepath, GLenum shader_type) {
    
    auto& shader_id = name_to_shader_map[name];

    if (!shader_id) {

        shader_id = name_to_shader_map.size();

        std::string shader_code;
        std::ifstream shader_file;
        std::stringstream shader_stream;

        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            shader_file.open(filepath);
            shader_stream << shader_file.rdbuf();
            shader_file.close();
            shader_code = shader_stream.str();
            //std:: cout << shader_code << "\n";
        } 
        catch (std::ifstream::failure&) {
            fmt::print("Failed to load shader \"{}\" at \"{}\"\n", name, filepath);
        }

        shader_id = glCreateShader(shader_type);
        const char* vertex_c_str = shader_code.c_str();
        glShaderSource(shader_id, 1, &vertex_c_str, NULL);
        glCompileShader(shader_id);
        
        int success;
        char log[1024];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if(success) {
            fmt::print("Loaded shader \"{}\" at \"{}\"\n", name, filepath);
            glGetShaderInfoLog(shader_id, 1024, NULL, log);
            fmt::print("{}", log);
        }
        else {
            glGetShaderInfoLog(shader_id, 1024, NULL, log);
            fmt::print("Failed to load shader \"{}\" at \"{}\"\n", name, filepath);
            fmt::print("{}", log);
        }
    }
}

void ShaderManager::add_program(std::string name, std::initializer_list<std::string> shaders) {
    auto &program_id = name_to_program_map[name];
    if (!program_id) {
        program_id = name_to_program_map.size();
        
        program_id = glCreateProgram();
        
        for (auto shader: shaders) {
            glAttachShader(program_id, name_to_shader_map[shader]);
        }
        glLinkProgram(program_id);

        int success;
        char log[1024];
        glGetProgramiv(program_id, GL_COMPILE_STATUS, &success);

        if(success) {

            for (auto shader: shaders) {
                glDetachShader(program_id, name_to_shader_map[shader]);
            }
            fmt::print("Loaded program \"{}\"\n", name);
            glGetProgramInfoLog(program_id, 1024, NULL, log);
            fmt::print("{}", log);
        }
        else {
            glGetProgramInfoLog(program_id, 1024, NULL, log);
            fmt::print("Failed to load program \"{}\"\n", name);
            fmt::print("{}", log);
        }
    }    
}

//@TODO REMOVE
void ShaderManager::use(std::string name) {
    auto& program_id = name_to_program_map[name];
    if (program_id) {
        glUseProgram(program_id);
    } else {
        throw new std::out_of_range("Program \"" + name + "\" not found");
    }
}

GLuint ShaderManager::get_program_id(std::string name) {

    auto& program_id = name_to_program_map[name];
    if (program_id) {
        return program_id;
    } else {
        fmt::print("Program not found {}\n", name);
        std::exit(1);
    }
}

void ShaderManager::delete_shaders() {

    for (auto [k, v] : name_to_shader_map) {
        glDeleteShader(v);
    }
}

void ShaderManager::delete_programs() {
    for (auto [k, v] : name_to_program_map) {
        fmt::print("program name: {} | program id: {}\n", k, v);
        glDeleteProgram(v);
    }
}

ShaderManager::~ShaderManager() {
    delete_shaders();
    delete_programs();
}

void ShaderManager::add_ubo(std::string name, size_t size, unsigned int binding) {

    auto& ubo_id = name_to_ubo[name];
    if (!ubo_id) {
        glGenBuffers(1, &ubo_id);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo_id);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void ShaderManager::bind_ubo(std::string program, std::string block_name, unsigned int binding) {
    auto id = name_to_program_map[program];
    auto block_index = glGetUniformBlockIndex(id, block_name.c_str());
    glUniformBlockBinding(id, block_index, binding);
}

GLuint ShaderManager::get_ubo(std::string name) {
    auto& ubo_id = name_to_ubo[name];
    if (ubo_id) {
        return ubo_id;
    } else {
        fmt::print("Ubo not found {}\n", name);
        std::exit(1);
    }
}