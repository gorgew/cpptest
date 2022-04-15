#include "ActionSequencer.hpp"
#include <fmt/printf.h>

void ActionSequencer::process(entt::registry& registry, float& delta_time) {

    for (auto& [id, ins_queue] : instruction_queue) {
        
        auto& curr = ins_queue.front();
        curr.first -= delta_time;
        fmt::print("curr: {} d: {}\n", curr.first, delta_time);

        if (curr.first <= 0.0f) {
            
            ins_queue.pop();
            if (ins_queue.empty()) {
                registry.remove<physics>(id);
                instruction_queue.erase(id);
            } else {
                auto& next = ins_queue.front();
                if (next.second.animation != "") 
                    resources->set_animation(registry, id, next.second.animation);
                registry.emplace_or_replace<physics>(id, next.second.velocity, next.second.acceleration);
            }
        }
    }
}

void ActionSequencer::set_path(entt::entity id, std::vector<std::pair<direction, float>>& path, float time) {
    std::queue<std::pair<float, instruction>> ins;
    float velocity = 1.7f;

    instruction temp;
    ins.push(std::make_pair(0.0f, temp));

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

        instruction i = {"", vel, glm::vec3(0.0f)};
        ins.push(std::pair(curr.second / 100.0f, i ));
    }
    
    
    instruction_queue[id] = ins;
}