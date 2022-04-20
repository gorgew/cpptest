#pragma once

#include "State.hpp"

#include <AudioSystem.hpp>
#include "TileMap.hpp"
#include "glm/glm.hpp"
#include "Camera.hpp"
#include "ScriptEngine.hpp"
#include "ResourceManager.hpp"
#include "ActionSequencer.hpp"

struct ui_character_data {
    std::string char_name;
    std::string label_name;
    struct nk_image profile_pic;
};

class StartState : public State {


    enum class substate {
        observe_world,
        character_select,
        lockout
    };

    enum class next_state {
        game_state,
        credits_state
    };
    std::shared_ptr<Camera> camera;

    TileMap tmap;

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

    substate m_substate = substate::observe_world;
    ActionSequencer action_seq;
    float lockout_timer = 0.0f;
    
    public:

        StartState(entt::registry& registry){
            build_gfx();
            
            build_music();
            
            build_scene(registry);

            key_system = {};
            mouse_system = {camera};

            build_key_handlers();
            build_mouse_handlers();

            link_scripts();
        };

        void handle_event(entt::registry& registry, SDL_Event e);
        void process_systems(entt::registry& registry, float& delta_time);
        std::shared_ptr<State> next(entt::registry& registry);

        void resize(int width, int height);

        void display_ui(nk_context* ctx);
        
        void set_lockout(float time);
        void lockout_transition(float& time);
};