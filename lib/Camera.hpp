#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {

    glm::mat4 view, projection;
    GLint view_loc, project_loc;
    GLuint program_id;

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    float yaw;
    float pitch;

    float camera_width;
    float camera_height;

    float pan_speed;
    float zoom_speed;

    float scale = 1.0f;

    void update_vectors();
    void update_view();

    public:
        Camera();
        Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint program_id);

        void pan(int x, int y, int z, float delta_time);

        void zoom(float zoom_val); 
        void resize(int width, int height);
};