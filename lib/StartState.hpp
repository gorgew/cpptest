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
    std::vector<std::string> active_skills;
    std::vector<std::string> passive_skills;
};

class StartState : public State {

private:
    class Substate {
        friend class StartState;
        private:
            virtual void build_key_handlers() {}
            virtual void build_mouse_handlers() {}
        protected:
            
            KeyEventSystem key_system;
            MouseEventSystem mouse_system;
            std::function<void(entt::registry&)> pan_right = [&](entt::registry& registry) mutable {
                s->camera->pan(1, 0, 0, s->delta_time);
            };

            std::function<void(entt::registry&)> pan_left = [=, this](entt::registry& registry) {
                s->camera->pan(-1, 0, 0, s->delta_time);
            };
            std::function<void(entt::registry&)> pan_up = [=, this](entt::registry& registry) {
                s->camera->pan(0, 1, 0, s->delta_time);
            };
            std::function<void(entt::registry&)> pan_down = [=, this](entt::registry& registry) {
                s->camera->pan(0, -1, 0, s->delta_time);
            };
            std::function<void(entt::registry&)> placeholder = [](entt::registry&){

            };

        public:
            
            Substate() {
                key_system = {};
                mouse_system = {s->camera};
            }
            virtual void enter(StartState* parent) = 0;
            virtual void exit(StartState* parent) = 0;
            void handle_event(entt::registry& registry, SDL_Event e);

            static StartState* s;
    };

    class ObserveSubstate : public Substate {
        friend class StartState;
        private:
            ObserveSubstate() {
                build_key_handlers(); 
                build_mouse_handlers();
            }
            void build_key_handlers();
            void build_mouse_handlers();
        public:
        
        static ObserveSubstate& instance();
        void enter(StartState* parent) {}
        void exit(StartState* parent) {}
        ObserveSubstate(ObserveSubstate const&) = delete;             // Copy construct
        ObserveSubstate(ObserveSubstate&&) = delete;                  // Move construct
        ObserveSubstate& operator=(ObserveSubstate const&) = delete;  // Copy assign
        ObserveSubstate& operator=(ObserveSubstate &&) = delete; 
    };
    friend class ObserveSubstate;

     class CharacterSelected : public Substate {
        friend class StartState;
        private:
            CharacterSelected() {
                build_key_handlers(); 
                build_mouse_handlers();
            }
            void build_key_handlers();
            void build_mouse_handlers();
        public:
        
        static CharacterSelected& instance();
        void enter(StartState* parent) {}
        void exit(StartState* parent) {}
        CharacterSelected(CharacterSelected const&) = delete;             // Copy construct
        CharacterSelected(CharacterSelected&&) = delete;                  // Move construct
        CharacterSelected& operator=(CharacterSelected const&) = delete;  // Copy assign
        CharacterSelected& operator=(CharacterSelected &&) = delete; 
    };
    friend class CharacterSelected;
    
    class SkillSelected : public Substate {
        friend class StartState;
        private:
            SkillSelected() {
                build_key_handlers(); 
                build_mouse_handlers();
            }
            void build_key_handlers();
            void build_mouse_handlers();
        public:
        
        static SkillSelected& instance();
        void enter(StartState* parent) {}
        void exit(StartState* parent) {}
        SkillSelected(SkillSelected const&) = delete;             // Copy construct
        SkillSelected(SkillSelected&&) = delete;                  // Move construct
        SkillSelected& operator=(SkillSelected const&) = delete;  // Copy assign
        SkillSelected& operator=(SkillSelected &&) = delete; 
    };
    friend class SkillSelected;

    Substate* _substate;

    void setState(Substate& s);
    
    enum class substate {
        observe_world,
        character_select,
        lockout,
        skill_target
    };
    
    enum class next_state {
        game_state,
        credits_state
    };

    skill_indicator_t skill_indicator;

    std::shared_ptr<Camera> camera;

    TileMap tmap{"art", "world", "billboard"};

    void build_key_handlers();
    void build_mouse_handlers();
    void build_music();
    void build_gfx();
    void build_scene(entt::registry& registry);
    void link_scripts();

    ui_character_data char_hover_data;

    void update_char_hover_data(entt::registry&);
    void show_character_hover_ui(nk_context* ctx);
    void show_env_ui(nk_context* ctx);
    void show_terrain_ui(nk_context* ctx);
    void display_demo(nk_context* ctx);

    bool ui_show_demo = false;
    bool ui_show_character_hover = false;
    bool ui_show_env = false;
    bool ui_show_terrain = false;

    substate m_substate = substate::observe_world;
    ActionSequencer action_seq;
    float lockout_timer = 0.0f;
    
    public:

        StartState(entt::registry& registry){
            build_gfx();
            
            build_music();
            
            build_scene(registry);
            Substate::s = this;
            _substate = &ObserveSubstate::instance();
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