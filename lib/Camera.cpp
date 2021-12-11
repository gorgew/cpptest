#include "Camera.hpp"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(){};

Camera::Camera(float win_width, float win_height, float pan_speed, float zoom_speed, GLuint program_id) {
    this->program_id = program_id;
    this->pan_speed = pan_speed;
    this->zoom_speed = zoom_speed;

    model_loc = glGetUniformLocation(program_id, "model");
    view_loc = glGetUniformLocation(program_id, "view");
    project_loc = glGetUniformLocation(program_id, "projection");

    camera_width = win_width;
    camera_height = win_height;

    projection = glm::ortho(0.0f, win_width, 0.0f, win_height);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::pan(float x, float y) {

    view = glm::translate(view, glm::vec3(pan_speed * x, pan_speed * y, 0.0f));

    glUseProgram(program_id);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::zoom(float zoom_val) {

    scale += zoom_speed * zoom_val;
    projection = glm::ortho(0.0f, camera_width * scale, 0.0f, camera_width * scale);

    glUseProgram(program_id);
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::resize(int width, int height) {
    camera_width = width;
    camera_height = height;

    projection = glm::ortho(0.0f, camera_width * scale, 0.0f, camera_width * scale);

    glUseProgram(program_id);
    glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
}
