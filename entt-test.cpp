#include <entt/entt.hpp>
#include <entt/core/hashed_string.hpp>
#include <iostream>

using namespace entt::literals;

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

void update(entt::registry &registry) {

    // Only look at entities with both position and velocity components
    auto view = registry.view<const position, velocity>();

    for(auto [entity, pos, vel] : view.each()) {
        
        std::cout << "position: (" << pos.x << ", " << pos.y << ")\n";
        std::cout << "velocity: (" << vel.dx << ", " << vel.dy << ")\n";
    }
}


int main(void) {
    
    entt::registry registry;
    
    // for (auto i = 0u; i < 10u; ++i) {
    //     const auto entity = registry.create();
    //     registry.emplace<position>(entity, i * 1.f, i *1.f);
    //     if (i % 2 == 0) {
    //         registry.emplace<velocity>(entity, i * .1f, i * .1f);
    //     }
    // }

    // update(registry);
    
    // auto factory = entt::meta<int>().type("asdf"_hs);
    // entt::meta<int>().data<1>("member"_hs);
    
    return 0;
}