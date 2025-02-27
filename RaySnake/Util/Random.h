#pragma once

#include <random>

namespace Random
{
    using RandomEngine = std::mt19937;

    RandomEngine GetRandomEngine();

    uint32_t GetRandomNumber(RandomEngine& engine, uint32_t max);
}
