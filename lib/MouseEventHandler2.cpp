#include "MouseEventHandler.hpp"

#include <iostream>

void MouseEventHandler::handle(SDL_Event e) {

    if (e.button.button == SDL_BUTTON_LEFT) {
        std::cout << "Left Mouse Down\n";
        for (auto &handler : left_handlers) {
            if (handler(e)) {
                break;
            }
        }
    }

    else if (e.button.button == SDL_BUTTON_RIGHT) {
        std::cout << "Right Mouse Down\n";
        for (auto &handler : right_handlers) {
            if (handler(e)) {
                break;
            }
        }
    }

    else if (e.button.button == SDL_BUTTON_MIDDLE) {
        for (auto &handler : middle_handlers) {
            if (handler(e)) {
                break;
            }
        }
    }
}