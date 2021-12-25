//Credit to learnopengl.com Camera class

#include "Camera.hpp"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(){};

Camera::Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint program_id) {
    this->program_id = program_id;
    this->pan_speed = pan_speed;
    this->zoom_speed = zoom_speed;

    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    yaw = -90.0f;
    pitch = 0.0f;

    view_loc = glGetUniformLocation(program_id, "view");
    project_loc = glGetUniformLocation(program_id, "projection");

    update_vectors();

    camera_width = win_width;
    camera_height = win_height;

    projection = glm::ortho(0.0f, win_width, 0.0f, win_height, -500.0f, 500.0f);

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::pan(int x, int y, int z, float delta_time) {

            float velocity = pan_speed * delta_time;
            if (x != 0) {
                velocity *= x;
                pos += right * velocity;
            }
            if (y != 0) {
                velocity *= y;
                pos += up * velocity;
            }
            if (z != 0) {
                velocity *= z;
                pos += front * velocity;
            }
            update_vectors();
        }

void Camera::zoom(float zoom_val) {
    /*
    scale += zoom_speed * zoom_val;
    projection = glm::ortho(0.0f, camera_width * scale, 0.0f, camera_width * scale);

    glUseProgram(program_id);
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
    */
}

void Camera::resize(int width, int height) {
    /*
    camera_width = width;
    camera_height = height;

    projection = glm::ortho(0.0f, camera_width * scale, 0.0f, camera_width * scale);

    glUseProgram(program_id);
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
    */
}

void Camera::update_view() {
    view = glm::lookAt(pos, pos + front, up);
    glUseProgram(program_id);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::update_vectors() {
    glm::vec3 temp_front;
    temp_front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    temp_front.y = sin(glm::radians(pitch));
    temp_front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
    front = glm::normalize(temp_front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(world_up);

    update_view();
}
