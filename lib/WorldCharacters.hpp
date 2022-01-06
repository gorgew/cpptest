// Store all characters that exist in the world
#pragma once
#include <string>
#include <vector>
/**
 * members are signed for updating
 */
struct character_stats {
    int aim;
    int max_health;
    int curr_health;
    int movement;
};


class character_registry {
    
    public:
        struct entry {
            std::string name;
            character_stats stats;
            unsigned int resource_id;
            std::vector<unsigned int> equipment;
            std::vector<unsigned int> skills;
            std::vector<unsigned int> abilities;
            std::vector<unsigned int> buffs;
        };
};
