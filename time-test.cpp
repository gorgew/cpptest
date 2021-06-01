#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <vector>
#include <ranges>
#include <numeric>
#include <chrono>

using namespace std::chrono;

float mean(std::vector<float> vec){
    return std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
}

float deviation(std::vector<float> vec) {
    float accum = 0.0;
    auto m = mean(vec);
    std::for_each (vec.begin(), vec.end(), [&](const float f) {
        accum += (f - m) * (f - m);
    });

    return sqrt(accum / (vec.size()-1));
}

void test_SDL() {
    float freq = SDL_GetPerformanceFrequency();

    float current_time, new_time, frame_time;
    std::vector<float> my_vec;
    for (int i  = 0; i < 100; ++i) {
        current_time = SDL_GetPerformanceCounter();
        SDL_Delay(33);
        new_time = SDL_GetPerformanceCounter();
        frame_time = (new_time - current_time) / freq * 1000.0f;
        my_vec.push_back(frame_time);
    }
    fmt::print("SDL_Delay / SDL PerformanceCounter Mean: {}\n", mean(my_vec));
    fmt::print("SDL_Delay / SDL PerformanceCounter StdDev: {}\n", deviation(my_vec));
}

void test_chrono() {

    auto clock = high_resolution_clock::now();
}

int main(void) {

    test_SDL();

    return 0;
}