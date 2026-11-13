//
// Created by dorish25 on 12.11.2026.
//

#include "RandomGenerator.h"
#include <random>

//lets twist again!
//init für mersenne twister
static std::mt19937 rng(std::random_device{}());

int RandomGenerator::randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

float RandomGenerator::randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
