#include "VertexArrayManager.hpp"

#include <fmt/format.h>

#include <numeric>
#include <stdexcept>
#include <cstdlib>

void VertexArrayManager::add_array(std::string name, float* data, GLsizei size,
        std::initializer_list<int> attribs) {

    GLuint &va_id = name_to_va_id_map[name];
    if (!va_id) {
        GLuint index = name_to_vb_id_map.size();

        va_id = index;
        name_to_vb_id_map[name] = index;
        GLuint &vb_id = name_to_vb_id_map[name];

        glGenVertexArrays(1, &va_id);
        glBindVertexArray(va_id);

        glGenBuffers(1, &vb_id);
        glBindBuffer(GL_ARRAY_BUFFER, vb_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 
        
        GLint attrib_count = attribs.size();
        int sum = std::accumulate(attribs.begin(), attribs.end(), 0);
        int stride_count = 0;
        for (int i = 0; i < attrib_count; ++i) {
                glVertexAttribPointer(i, attrib_count, GL_FLOAT, GL_FALSE, sum * sizeof(float), 
                        (void*) (stride_count * sizeof(float)));       
                glEnableVertexAttribArray(i);
                stride_count += std::data(attribs)[i];
        }
    }
}

GLuint VertexArrayManager::get_array_id(std::string name) {

    GLuint id = name_to_va_id_map[name];
    if (id) {
        return id;
    } else {
        fmt::print("VAO not found: {}\n", name);
        std::exit(1);
    }
}

VertexArrayManager::~VertexArrayManager() {
    for (auto [k,v] : name_to_va_id_map) {
        glDeleteVertexArrays(1, &v);
    }
    for (auto [k,v] : name_to_vb_id_map) {
        glDeleteBuffers(1, &v);
    }
}

void VertexArrayManager::add_rect(std::string name, float width, float height) {

        float data[20] = {
            //Bottom left corner
            -1 * (width / 2.0f), -1 * (height / 2.0f), 0.0f, 0.0f, 0.0f,
            //Bottom right corner
            width / 2.0f, -1 * (height / 2.0f), 0.0f, 1.0f, 0.0f,
            //Top right corner
            width / 2.0f, height / 2.0f, 0.0f, 1.0f, 1.0f,
            //Top left corner
            -1.0f * (width / 2.0f), height / 2.0f, 0.0f, 0.0f, 1.0f,
        };

        add_array(name, data, 20 * sizeof(float), {3, 2});
}

void VertexArrayManager::add_rect_flipped(std::string name, float width, float height) {

        float data[20] = {
            //Bottom left corner
            -1 * (width / 2.0f), -1 * (height / 2.0f), 0.0f, 0.0f, 1.0f,
            //Bottom right corner
            width / 2.0f, -1 * (height / 2.0f), 0.0f, 1.0f, 1.0f,
            //Top right corner
            width / 2.0f, height / 2.0f, 0.0f, 1.0f, 0.0f,
            //Top left corner
            -1.0f * (width / 2.0f), height / 2.0f, 0.0f, 0.0f, 0.0f,
        };

        add_array(name, data, 20 * sizeof(float), {3, 2});
}