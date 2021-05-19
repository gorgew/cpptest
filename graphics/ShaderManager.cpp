#include <ShaderManager.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

void ShaderManager::add_shader(std::string name, std::string filepath) {

    name_to_shader_map.insert({name, 0});

    std::string shader_code;
    std::ifstream shader_file;
    std::stringstream shader_stream;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(filepath);
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    } 
    catch (std::ifstream::failure) {
        
        std::cout << "shader opening error\n";
    }
    
    std::cout<<"Compiling vertex shader\n";
    name_to_shader_map[name] = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_c_str = shader_code.c_str();
    glShaderSource(name_to_shader_map[name], 1, &vertex_c_str, NULL);
    glCompileShader(name_to_shader_map[name]);
    check_shader(name_to_shader_map[name]);
}

bool ShaderManager::check_shader(GLuint shader_id) {

    int success;
    char log[1024];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shader_id, 1024, NULL, log);
        std::cerr << "PROGRAM LINKING ERROR:\n" << log;
        return true;
    }
    return false;
}