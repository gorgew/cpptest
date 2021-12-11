#pragma once

#include <vector>
#include <robin_hood/robin_hood.h>

struct actor_component {
    unsigned int index; // Index at some array
    unsigned int hp;
};

struct actor {

    unsigned int hp;
    unsigned int movement;
    
    unsigned int weapon_id;
    unsigned int armor_id;
    //....
    robin_hood::unordered_set<unsigned int> auras;
    robin_hood::unordered_set<unsigned int> passives;
    
    std::vector<unsigned int> abilities;

};
