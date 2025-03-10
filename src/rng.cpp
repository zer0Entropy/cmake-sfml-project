#include "../include/rng.hpp"

RandomNumberGenerator::RandomNumberGenerator():
    seedSource{},
    mersenneTwisterEngine{seedSource()} {

}

RandomNumberGenerator::~RandomNumberGenerator() {

}

unsigned int RandomNumberGenerator::GetRandom(unsigned int min, unsigned int max) {
    std::uniform_int_distribution<>     distribution(min, max);
    return distribution(mersenneTwisterEngine);
}
    