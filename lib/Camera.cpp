//Credit to learnopengl.com Camera class

#include "Camera.hpp"
#include "UboStructs.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(){};

Camera::Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint camera_ubo) {

    this->pan_speed = pan_speed;
    this->zoom_speed = zoom_speed;
    this->camera_ubo = camera_ubo;

    pos = glm::vec3(-100.0f, -100.0f, 0.0f);
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    
    
    yaw = -45.0f;
    pitch = 35.0f;
    roll = 315.0f;
    
    /*
   yaw = -90.0f;
   pitch = 0.0f;
   roll = 0.0f;
    */
   
    view_offset = offsetof(camera_data, view);
    project_offset = offsetof(camera_data, projection);

    update_vectors();

    camera_width = win_width;
    camera_height = win_height;

    projection = glm::ortho(-win_width, win_width, -win_height, win_height, ortho_near, ortho_far);
    update_project();
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
            update_view();

        }

void Camera::zoom(float zoom_val, float delta_time) {

    scale += zoom_val * delta_time * zoom_speed;
    scale = std::abs(scale);
    update_project();
}

void Camera::resize(int width, int height) {
    camera_width = width;
    camera_height = height;
    update_project();
}

void Camera::update_view() {
    view = glm::lookAt(pos, pos + front, up);

    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, view_offset, sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::update_project() {
    float scaled_width = camera_width * scale;
    float scaled_height = camera_height * scale;
    projection = glm::ortho(-scaled_width, scaled_height, -scaled_height, scaled_height, ortho_near, ortho_far);

    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, project_offset, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::update_vectors() {
    glm::vec3 temp_front;
    temp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    temp_front.y = sin(glm::radians(pitch));
    temp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(temp_front);

    world_up = glm::rotate(world_up, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
    
    roll = 0.0f;
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));

    update_view();
}

void Camera::update_pitch(float value) {
    pitch += value;
    
    if (pitch > 89.9f) {
        pitch = 89.9f;
    }
    else if (pitch < -89.9f) {
        pitch = -89.9f;
    }
    update_vectors();
}

void Camera::update_yaw(float value) {
    yaw += value;
    /*
    if (yaw > 89.9f) {
        yaw = 89.9f;
    }
    else if (yaw < -89.9f) {
        yaw = -89.9f;
    }*/
    update_vectors();
}

void Camera::center(float x, float y) {
    pos = glm::vec3(x, y, pos.z);
}

void Camera::process_events(const float& delta_time) {
    event_accum += static_cast<int>(delta_time);
    const auto& top = event_queue.front();
    if (event_accum > top.delay) {
        event_accum -= top.delay;

        using enum camera_event_t;

        event_queue.pop();
    } 
}

glm::vec3 Camera::get_position() {
    return pos + up;
}

glm::mat4 Camera::get_projection() {
    return projection;
}

glm::mat4 Camera::get_view() {
    return view;
}