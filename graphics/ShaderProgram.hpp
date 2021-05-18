#pragma once

extern "C" {

    #include <glad/glad.h>
}

#include <string>
#include <glad/glad.h>

class ShaderProgram {

    private:

        GLuint programID;

        void checkCompileStatus(GLuint ID, bool isShader);

    public: 
        
        ShaderProgram(const char* vertexFile, const char* fragmentFile);

        void use();

        void close();

        void setUniform(const std::string &name, bool value) const;

        void setUniform(const std::string &name, int value) const;

        void setUniform(const std::string &name, float value) const;

        void setUniform(const std::string &name, float v1, float v2) const;

        void setUniform(const std::string &name, float v1, float v2, float v3) const;

        void setUniformMatrix3(const std::string &name, GLboolean transpose, const GLfloat *value);

        void setUniform(const std::string &name, float v1, float v2, float v3, float v4) const;
        
        void setUniformMatrix4(const std::string &name, GLboolean transpose, const GLfloat *value);

        GLuint getID();
};