#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>

class VertexArrayManager {

    std::unordered_map<std::string, GLuint> name_to_vb_id_map;
    std::unordered_map<std::string, GLuint> name_to_va_id_map;
    
    public:
        VertexArrayManager() = default;
        /**
         * @brief Create a vertex array of type float
         * @param name Name of vertex array
         * @param data Pointer to data
         * @param size Size of data
         * @param attribs List of sizes of attributes
         * */
        void add_array(std::string name, float* data, GLsizei size, 
                std::initializer_list<int> attribs);

        /**
         * @brief Create a vertex array of a rect of the given width and height.
         *  The rect should be drawn with GL primitive type of GL_TRIANGLE_FAN
         * @param width Width of rect
         * @param height Height of rect
         */
        void add_rect(std::string name, float width, float height);
        
        /**
         * @brief Get the id of a VAO mapped to the given name
         * @param name Name of VAO
         * @return id of VAO. 0 if VAO not found
        */
        GLuint get_array_id(std::string name);

        ~VertexArrayManager();
};