#include "ActionSequencer.hpp"
#include "PhysicsComponents.hpp"
#include <fmt/printf.h>

void ActionSequencer::process(entt::registry& registry, float& delta_time) {

    auto ins_view = registry.view<position, action_instruction>();
    for (auto [id, pos, ins] : ins_view.each()) {
        float next = ins.time - delta_time;
        if (next <= 0) {
            ins.velocity += (ins.acceleration * ins.time);
            pos.pos += (ins.velocity * ins.time);

            //set next values
            auto& q = instruction_queue[id];
            if (q.empty()) {
                instruction_queue.erase(id);
                registry.remove<action_instruction>(id);
            }
            else {
                auto& next_ins = q.front();
                ins.time = next_ins.time;
                ins.velocity = next_ins.velocity;
                ins.acceleration = next_ins.acceleration;
                if (next_ins.animation != "") {
                    resources->set_animation(registry, id, next_ins.animation);
                }
                q.pop();
            }            

        }
        else {
            ins.velocity += (ins.acceleration * delta_time);
            pos.pos += (ins.velocity * delta_time);
            ins.time = next;
        }
    }
}

float ActionSequencer::set_path(entt::registry& registry, entt::entity id, std::vector<std::pair<direction, float>>& path, float time) {
    std::queue<instruction> ins;
    float velocity = 100.0f / time;

    registry.emplace<action_instruction>(id, 0.0f, glm::vec3(0.0f), glm::vec3(0.0f));
    float acc_time = 0.0f;
    for (auto& curr : path) {

        auto vel = glm::vec3(0.0f);
        if (curr.first == direction::up) {
            vel = glm::vec3(0.0f, velocity, 0.0f);  
        }
        else if (curr.first == direction::down) {
            vel = glm::vec3(0.0f, -velocity, 0.0f);
        }
        else if (curr.first == direction::right) {
            vel = glm::vec3(velocity, 0.0f, 0.0f);
        }
        else if (curr.first == direction::left) {
            vel = glm::vec3(-velocity, 0.0f, 0.0f);
        }
        float step_time = curr.second / 100.0f * time;
        acc_time += step_time;
        instruction i = {step_time, "", vel, glm::vec3(0.0f)};
        ins.push(i);
    }
    
    instruction_queue[id] = ins;
    return acc_time;
}