//
// Created by filip on 12.02.25.
//

#ifndef RANDOM_H
#define RANDOM_H
#include <random>

namespace Random
{
    using RandomEngine = std::mt19937;

    RandomEngine GetRandomEngine();

    uint32_t GetRandomNumber(RandomEngine& engine, uint32_t max);
}

#endif //RANDOM_H
