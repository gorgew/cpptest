#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {

    glm::mat4 view, model, projection;
    GLint view_loc, model_loc, project_loc;
    GLuint program_id;
    
    float camera_width;
    float camera_height;
    
    float pan_speed;
    float zoom_speed;

    float scale = 1.0f;

    public:
        Camera();
        Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint program_id);

        void pan(float x, float y);

        void zoom(float zoom_val); 
        void resize(int width, int height);
};