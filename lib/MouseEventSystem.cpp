#include "MouseEventSystem.hpp"
#include <fmt/format.h>
#include <PhysicsComponents.hpp>

MouseEventSystem::MouseEventSystem(std::shared_ptr<Injector> injector, std::shared_ptr<Camera> camera) {
    this->injector = injector;
    this->camera = camera;
}

float MouseEventSystem::world_x_to_ndc(float x) {
    return (2.0f * x) / injector->config.width - 1.0f;
}

float MouseEventSystem::world_y_to_ndc(float y) {
    return 1.0f  - (2.0f * y) / injector->config.height;
}

void MouseEventSystem::handle_event(entt::registry& registry, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        
        set_world_coords(e.button.x, e.button.y);

        if (e.button.button == SDL_BUTTON_LEFT) {
            leftmouse_down(registry);
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            rightmouse_down(registry);
        }

        //check_rect_buttons(registry, gl_x, gl_y);
    }
    else if (e.type == SDL_MOUSEWHEEL) {

        if (e.wheel.y > 0) {
            wheel_up_handler(registry);
        }
        else {
            wheel_down_handler(registry);
        }
    }
    else if (e.type == SDL_MOUSEMOTION) {
        set_world_coords(e.motion.x, e.motion.y);
        motion_handler(registry);
    }
};

void MouseEventSystem::check_rect_buttons(entt::registry& registry, float gl_x, float gl_y) {

    if (injector->config.debug) {
        //fmt::print("Checking rect buttons\n");
    }

    auto view = registry.view<position, rect_button>();

    for (auto [entity, pos, button] : view.each()) {
        
        //Calculate world location of min/max x's and y's
        float world_min_x = pos.pos.x - button.width / 2.0f;
        float world_min_y = pos.pos.y + button.length / 2.0f;
        float world_max_x = pos.pos.x + button.width / 2.0f;
        float world_max_y = pos.pos.y - button.length / 2.0f;
        fmt::print("world_min_x: {}, world_max_x: {}, world_min_y: {}, world_max_y: {}\n", world_min_x, world_max_x,world_min_y, world_max_y);
        float gl_min_x = world_x_to_ndc(world_min_x);
        float gl_max_x = world_x_to_ndc(world_max_x);
        float gl_min_y = world_y_to_ndc(world_min_y);
        float gl_max_y = world_y_to_ndc(world_max_y);
        fmt::print("gl_min_x: {}, gl_max_x: {}, gl_min_y: {}, gl_max_y: {}\n", gl_min_x, gl_max_x, gl_min_y, gl_max_y);
        if (gl_x >= gl_min_x && gl_x <= gl_max_x && gl_y >= gl_min_y && gl_y <= gl_max_y) {
            if (injector->config.debug) {
                fmt::print("Button down\n");
            }
            button.event(registry);
        }
        
    }
}

void MouseEventSystem::check_rect_handlers(entt::registry& registry, Uint8 type, float gl_x, float gl_y) {

    for (auto handler : rect_handlers) {
        
        if (handler.type == type) {

            if (gl_x >= handler.min_x && gl_x <= handler.max_x && 
                gl_y >= handler.min_y && gl_y <= handler.max_y) {
                
                handler.action(registry);
            }
        }
    }
};

void MouseEventSystem::add_rect_button(std::function<void(entt::registry&)> handler, 
        Uint8 type, int min_x, int min_y, int max_x, int max_y) {
    
    add_rect_button(handler, type, world_x_to_ndc(min_x), world_y_to_ndc(min_y), 
            world_x_to_ndc(max_x), world_y_to_ndc(max_y));
}

void MouseEventSystem::add_rect_button(std::function<void(entt::registry&)> handler, 
        Uint8 type, float min_x, float min_y, float max_x, float max_y) {
    
    
}

void MouseEventSystem::add_wheel_handler(bool is_up, std::function<void(entt::registry&)> handler) {
    if (is_up) {
        wheel_up_handler = handler;
    }
    else {
        wheel_down_handler = handler;
    }
}

void MouseEventSystem::add_mousedown_handler(std::function<void(entt::registry&)> handler, char button) {
    if (button == SDL_BUTTON_LEFT) {
        leftmouse_down = handler;
    }
    else if (button == SDL_BUTTON_RIGHT) {
        rightmouse_down = handler;
    }
}

void MouseEventSystem::set_world_coords(int x, int y) {
    
    glm::vec4 ray_proj = glm::inverse(camera->get_projection()) * 
        glm::vec4(world_x_to_ndc(x), world_y_to_ndc(y), -1.0f, 1.0f);
    ray_proj.z = -1.0f;
    ray_proj.w = 1.0f;
    glm::vec3 ray_view = glm::vec3((glm::inverse(camera->get_view()) * ray_proj));

    //solving for intersection between line from camera to plane z = 0
    
    float t = -1 * ray_view.z / camera->front.z;
    float intersect_x = camera->front.x * t + ray_view.x;
    float intersect_y = camera->front.y * t + ray_view.y;

    world_x = static_cast<unsigned int>(intersect_x);
    world_y = static_cast<unsigned int>(intersect_y);
    
    if (injector->config.debug) {
        /*
        fmt::print("Mouse: sdl coords: ({}, {}), gl coords: ({}, {})\n", e.button.x, e.button.y, gl_x, gl_y);
        fmt::print("Mouse ray cast: ({}, {}, {})\n", ray_view.x, ray_view.y, ray_view.z);
        fmt::print("Intersect at ({}, {})\n", intersect_x, intersect_y);
        */
    }
}

void MouseEventSystem::set_motion_handler(std::function<void(entt::registry&)> handler) {
    motion_handler = handler;
}