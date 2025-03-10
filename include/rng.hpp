#pragma once

#include <random>

class RandomNumberGenerator {
public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    unsigned int            GetRandom(unsigned int min, unsigned int max);
    
private:
    std::random_device      seedSource;
    std::mt19937            mersenneTwisterEngine;
};