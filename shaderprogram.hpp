#ifndef SHADERPROGRAM
#define SHADERPROGRAM

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

class ShaderProgram {
    public: 
        
        ShaderProgram(const char* vertexFile, const char* fragmentFile) {
            
            std::string vertexShaderCode, fragmentShaderCode;
            std::ifstream vertexShaderFile, fragmentShaderFile;
            std::stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                vertexShaderFile.open(vertexFile);
                fragmentShaderFile.open(fragmentFile);
                
                vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf();

                vertexShaderFile.close();
                fragmentShaderFile.close();

                vertexShaderCode = vertexShaderStream.str();
                fragmentShaderCode = fragmentShaderStream.str();

            } catch (std::ifstream::failure) {
                std::cerr<<"SHADER FILE OPENING ERROR";
            }
            
            std::cout<<"Compiling vertex shader\n";
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vertexCStr = vertexShaderCode.c_str();
            glShaderSource(vertexShader, 1, &vertexCStr, NULL);
            glCompileShader(vertexShader);
            checkCompileStatus(vertexShader, true);

            std::cout<<"Compiling fragment shader\n";
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragmentCStr = fragmentShaderCode.c_str();
            glShaderSource(fragmentShader, 1, &fragmentCStr, NULL);
            glCompileShader(fragmentShader);
            checkCompileStatus(fragmentShader, true);

            programID = glCreateProgram();
            glAttachShader(programID, vertexShader);
            glAttachShader(programID, fragmentShader);
            glLinkProgram(programID);
            checkCompileStatus(programID, false);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void use() {
            
            glUseProgram(programID);

        }

        void close() {
            
            glDeleteProgram(programID);

        }

        void setUniform(const std::string &name, bool value) const {
           
            glUniform1i(glGetUniformLocation(programID, name.c_str()), (GLboolean) value);

        }

        void setUniform(const std::string &name, int value) const {
            
            glUniform1i(glGetUniformLocation(programID, name.c_str()), (GLint) value);

        }

        void setUniform(const std::string &name, float value) const {
           
            glUniform1f(glGetUniformLocation(programID, name.c_str()), (GLfloat) value);

        }

        void setUniform(const std::string &name, float v1, float v2) const {
           
            glUniform2f(glGetUniformLocation(programID, name.c_str()), (GLfloat) v1, (GLfloat) v2);

        }

        void setUniform(const std::string &name, float v1, float v2, float v3) const {
            
            glUniform3f(glGetUniformLocation(programID, name.c_str()), (GLfloat) v1, (GLfloat) v2, 
                (GLfloat) v3);

        }

        void setUniform(const std::string &name, float v1, float v2, float v3, float v4) const {
           
            glUniform4f(glGetUniformLocation(programID, name.c_str()), (GLfloat) v1, (GLfloat) v2, 
                (GLfloat) v3, (GLfloat) v4);
                
        }
        
        void setUniformMatrix4(const std::string &name, GLboolean transpose, const GLfloat *value) {
            
            glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, transpose, value);

        }

        GLuint getID() {
            return programID;
        }

    private:

        GLuint programID;

        void checkCompileStatus(GLuint ID, bool isShader) {

            int success;
            char infoLog[1024];
            if(!(isShader)) {
                glGetProgramiv(ID, GL_LINK_STATUS, &success);
                if(!success) {
                    glGetShaderInfoLog(ID, 1024, NULL, infoLog);
                    std::cerr << "SHADER COMPILATION ERROR:\n" << infoLog;
                }

            } else {
                glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
                if(!success) {
                    glGetProgramInfoLog(ID, 1024, NULL, infoLog);
                    std::cerr << "PROGRAM LINKING ERROR:\n" << infoLog;
                }
            }
        }
};

#endif