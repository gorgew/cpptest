#include <ShaderManager.hpp>

#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

void ShaderManager::add_shader(std::string name, std::string filepath, GLenum shader_type) {

    name_to_shader_map.insert({name, 0});
    GLuint& shader_id = name_to_shader_map[name];

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
    catch (std::ifstream::failure) {
        std::cout << "Failed to load shader \"" <<  name << "\" at \"" << filepath << "\"\n";
        //fmt::print("Failed to load shader \"{}\" at \"{}\"\n", name, filepath);
    }

    shader_id = glCreateShader(shader_type);
    const char* vertex_c_str = shader_code.c_str();
    glShaderSource(shader_id, 1, &vertex_c_str, NULL);
    glCompileShader(shader_id);
    
    int success;
    char log[1024];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if(success) {
        std::cout << "Loaded shader \"" <<  name << "\" at \"" << filepath << "\"\n";
        glGetShaderInfoLog(shader_id, 1024, NULL, log);
        std::cout << log << "\n";
    }
    else {
        glGetShaderInfoLog(shader_id, 1024, NULL, log);
        std::cout << "Failed to compile shader \"" <<  name << "\" at \"" << filepath << "\"\n";
        std::cout << log << "\n";
    }

}

void ShaderManager::add_program(std::string name, std::initializer_list<std::string> programs) {

    name_to_program_map.insert({name, 4});
    GLuint& program_id = name_to_program_map[name];
    program_id = glCreateProgram();

    for (auto program: programs) {
        glAttachShader(program_id, name_to_shader_map[program]);
    }
    glLinkProgram(program_id);

    int success;
    char log[1024];
    glGetProgramiv(program_id, GL_COMPILE_STATUS, &success);

    if(success) {
        std::cout << "Loaded program \"" <<  name << "\"\n";
        glGetProgramInfoLog(program_id, 1024, NULL, log);
        std::cout << log << "\n";
    }
    else {
        glGetProgramInfoLog(program_id, 1024, NULL, log);
        std::cout << "Failed to link program\"" <<  name << "\"\n";
        std::cout << log << "\n";
    }

    bool x = glIsProgram(program_id);
    std::cout << "is manager program: " << x << "\n";
}

void ShaderManager::use(std::string name) {

    if (name_to_program_map.contains(name)) {
        glUseProgram(name_to_program_map[name]);
    } else {
        throw new std::out_of_range("Program \"" + name + "\" not found");
    }
}

GLuint& ShaderManager::get_program_id(std::string name) {
    
    if (name_to_program_map.contains(name)) {
        GLuint &id = name_to_program_map[name];
        return id;
    } else {
        throw new std::out_of_range("Program \"" + name + "\" not found");
    }
}

void ShaderManager::delete_shaders() {

    for (auto [k, v] : name_to_shader_map) {
        glDeleteShader(v);
    }
}

void ShaderManager::delete_programs() {
    for (auto [k, v] : name_to_program_map) {
        glDeleteProgram(v);
    }
}

