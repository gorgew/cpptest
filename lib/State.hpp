#pragma once

#include <entt/entt.hpp>
#include "KeyEventSystem.hpp"
#include "MouseEventSystem.hpp"
#include <memory>
#include "Injector.hpp"
#include "SDL2/SDL.h"
#include <glm/glm.hpp>


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#include <nuklear/nuklear.h>

class State {
    
    //private:
    //Other systems go here:
    //physics, whatever

    protected:
        GLuint program_id;

        //Specific handlers and what not
        KeyEventSystem key_system;
        MouseEventSystem mouse_system;
        std::shared_ptr<Injector> injector;
        

        bool go_next = false;
        bool systems_enabled = true;

        virtual void build_key_handlers(){};
        virtual void build_mouse_handlers(){};
        virtual void build_music(){};
        virtual void build_gfx(){};
        virtual void build_scene(entt::registry& registry){};

    public:

        float delta_time;
        
        //Initailize the base stuff
        State(){};
        
        
        virtual void handle_event(entt::registry& registry, SDL_Event e) {
            key_system.handle_event(registry, e);
            mouse_system.handle_event(registry, e);
        };

        //Go through all the systems
        virtual void process_systems(entt::registry& registry) = 0;

        /**
         * @brief Yikes!
        */
        bool ready_next() {
            return go_next;
        };
        //Go to the next state
        virtual std::shared_ptr<State> next(entt::registry& registry) = 0;
        
        virtual void display_ui(nk_context* ctx) = 0;
        virtual void resize(int width, int height) = 0;
        virtual ~State() {}
};