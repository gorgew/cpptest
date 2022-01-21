#include <fmt/printf.h>
#include <random>

struct weapon {
    int power;
    float attack_chance;
    float block_chance;
    float counter_chance;
    float parry_chance;
};

weapon sword = {3, .8f, .3f, .3f, .2f};

enum class attack_t {
    engage,
    counter,
    parry
};

void simulate(int& hp_1, int& hp_2, weapon& w_1, weapon& w_2, auto& gen, auto& dist) {

    if (dist(gen) < w_1.attack_chance) {
        fmt::print("Attack hits!\n");
        if (dist(gen) < w_2.block_chance) {
            fmt::printf("... but it gets blocked!\n");
        }
        else {
            hp_2 -= w_1.power;
        }
    }
    else {
        fmt::print("Attack misses!\n");
        if (dist(gen) < w_2.counter_chance) {
            fmt::print("...Counterattack!\n");
            //hp_1 -= w_2.power;
            simulate(hp_2, hp_1, w_2, w_1, gen, dist);
        }
    }
}

int main(void) {

    
    std::random_device r;
    std::mt19937 rng(r());
    std::uniform_real_distribution<> dist(0.0f, 1.0f);
    //fmt::print("float: {}\n", (float) dist(rng));
    weapon w_1 = sword;
    weapon w_2 = sword;

    bool p1_turn = true;

    for (int i = 0; i < 1; i++) {
        int hp_1 = 10, hp_2 = 10;
        while (hp_1 > 0 && hp_2 > 0) {
            if (p1_turn) {
                fmt::print("p1_turn:\n");
                p1_turn = !p1_turn;
                simulate(hp_1, hp_2, w_1, w_2, rng, dist);
            }
            else {
                fmt::print("p2_turn:\n");
                simulate(hp_2, hp_1, w_2, w_1, rng, dist);
                p1_turn = !p1_turn;
            }
            //hp_1--;
        }
        if (hp_1 > 0) {
            fmt::print("p1 wins!\n");
        }
        else {
            fmt::print("p2 wins!\n");
        }      
    }
    

    return 0;
}