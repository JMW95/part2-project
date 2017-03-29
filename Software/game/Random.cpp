#include "Random.h"

#include <random>

//static std::random_device r;
static std::default_random_engine engine(0);

int Random::get_int(int a, int b){
    std::uniform_int_distribution<int> rand(a, b);
    return rand(engine);
}

float Random::get_float(float a, float b){
    std::uniform_real_distribution<float> rand(a, b);
    return rand(engine);
}