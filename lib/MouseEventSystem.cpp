#include "MouseEventSystem.hpp"
#include <fmt/format.h>

MouseEventSystem::MouseEventSystem(std::shared_ptr<Injector> injector) {
    this->injector = injector;
}

void MouseEventSystem::handle_event(entt::registry& registry, SDL_Event e) {

    float gl_x = (2.0f * e.button.x) / injector->config.width - 1.0f;
    float gl_y = 1.0f  - (2.0f * e.button.y) / injector->config.height;
    fmt::print("Mouse: sdl coords: ({}, {}), gl coords: ({}, {})", e.button.x, e.button.y, gl_x, gl_y);
    check_rect_handlers(registry, e.button.button, gl_x, gl_y);
};

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

