#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {

    glm::mat4 view, projection;

    size_t view_offset, project_offset;
    GLuint camera_ubo;
    glm::vec3 pos;
    
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    float yaw;
    float pitch;
    float roll;

    float camera_width;
    float camera_height;

    float ortho_near = -10000.0f;
    float ortho_far = 10000.0f;
    float pan_speed;
    float zoom_speed;

    float scale = 1.0f;

    void update_vectors();
    void update_view();
    void update_project();
    
    public:
        
        Camera();
        Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint camera_ubo);

        void pan(int x, int y, int z, float delta_time);
        void update_yaw(float value);
        void update_pitch(float value);
        void zoom(float zoom_val, float delta_time); 
        void resize(int width, int height);
        glm::vec3 get_position();
        glm::mat4 get_view();
        glm::mat4 get_projection();

        glm::vec3 front;
};