#pragma once

#include "State.hpp"

#include <AudioSystem.hpp>
#include "Injector.hpp"
#include "TileMap.hpp"
#include "glm/glm.hpp"
#include "Camera.hpp"
#include "ScriptEngine.hpp"
#include "ResourceManager.hpp"

struct ui_character_data {
    std::string char_name;
    std::string label_name;
    struct nk_image profile_pic;
};

class StartState : public State {

    enum class next_state {
        game_state,
        credits_state
    };
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ScriptEngine> scripts;
    std::shared_ptr<ResourceManager> resources;

    TileMap2D tmap;

    void build_key_handlers();
    void build_mouse_handlers();
    void build_music();
    void build_gfx();
    void build_scene(entt::registry& registry);
    void link_scripts();

    ui_character_data char_hover_data;

    void update_char_hover_data(entt::registry&);
    void show_character_hover_ui(nk_context* ctx);
    void display_demo(nk_context* ctx);

    bool ui_show_demo = false;
    bool ui_show_character_hover = false;
    
    public:

        StartState(std::shared_ptr<Injector> injector, entt::registry& registry, 
            std::shared_ptr<ScriptEngine> scripts, std::shared_ptr<ResourceManager> resources){

            this->injector = injector;
            this->scripts = scripts;
            this->resources = resources;
            
            build_gfx();
            
            build_music();
            
            build_scene(registry);

            key_system = {};
            mouse_system = {injector, camera};

            build_key_handlers();
            build_mouse_handlers();

            link_scripts();
        };
        void process_systems(entt::registry& registry);
        std::shared_ptr<State> next(entt::registry& registry);

        void resize(int width, int height);

        void display_ui(nk_context* ctx);
};