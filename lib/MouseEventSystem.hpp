#pragma once

#include <Injector.hpp>
#include <Camera.hpp>
#include <SDL2/SDL_events.h>
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include "WidgetComponent.hpp"

struct rect_clickable {

    Uint8 type;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    std::function<void(entt::registry&)> action;
};

class MouseEventSystem {

    std::vector<rect_clickable> rect_handlers;
    std::shared_ptr<Injector> injector;
    std::shared_ptr<Camera> camera;

    float world_x_to_ndc(float x);
    float world_y_to_ndc(float y);

    void check_rect_handlers(entt::registry& registry, Uint8 type, float gl_x, float gl_y);

    void check_rect_buttons(entt::registry& registry, float gl_x, float gl_y);
    void set_world_coords(int x, int y);
    std::function<void(entt::registry&)> wheel_up_handler;
    std::function<void(entt::registry&)> wheel_down_handler;

    std::function<void(entt::registry&)> leftmouse_down;
    std::function<void(entt::registry&)> rightmouse_down;
    std::function<void(entt::registry&)> motion_handler;

    public:
        MouseEventSystem() = default;
        MouseEventSystem(std::shared_ptr<Injector> injector, std::shared_ptr<Camera> camera);
        void handle_event(entt::registry& registry, SDL_Event e);
        void add_rect_button(std::function<void(entt::registry&)> handler, Uint8 type, int min_x, int min_y, int max_x, int max_y);
        void add_rect_button(std::function<void(entt::registry&)> handler, Uint8 type, float min_x, float min_y, float max_x, float max_y);
        
        //is_up: whether mouse wheel is scrolling up
        void add_wheel_handler(bool is_up, std::function<void(entt::registry&)> handler);

        void add_mousedown_handler(std::function<void(entt::registry&)> handler, char button);
        
        void set_motion_handler(std::function<void(entt::registry&)> handler);

        unsigned int world_x;
        unsigned int world_y;
};