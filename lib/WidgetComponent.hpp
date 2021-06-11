#pragma once
#include <functional>
#include <entt/entity/registry.hpp>

/**
 * @brief Bounding box of button to check against
*/
struct rect_button {
    float width;
    float length;
    std::function<void(entt::registry&)> event;
};
