#pragma once

#include <Injector.hpp>
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

    float world_x_to_ndc(float x);
    float world_y_to_ndc(float y);

    void check_rect_handlers(entt::registry& registry, Uint8 type, float gl_x, float gl_y);

    void check_rect_buttons(entt::registry& registry, float gl_x, float gl_y);
    public:
        MouseEventSystem() = default;
        MouseEventSystem(std::shared_ptr<Injector> injector);
        void handle_event(entt::registry& registry, SDL_Event e);
        void add_rect_button(std::function<void(entt::registry&)> handler, Uint8 type, int min_x, int min_y, int max_x, int max_y);
        void add_rect_button(std::function<void(entt::registry&)> handler, Uint8 type, float min_x, float min_y, float max_x, float max_y);
};