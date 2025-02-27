#include "Random.h"

#include <algorithm>
#include <array>
#include <cstdint>


namespace Random
{
    RandomEngine GetRandomEngine()
    {
        std::array<uint32_t, std::mt19937::state_size> RandomSeeds;
        std::random_device RandomDevice;
        std::generate(RandomSeeds.begin(), RandomSeeds.end(), std::ref(RandomDevice));
        std::seed_seq Seeding(RandomSeeds.begin(), RandomSeeds.end());

        return RandomEngine(Seeding);

    }

    uint32_t GetRandomNumber(RandomEngine& engine, uint32_t max)
    {
        std::uniform_int_distribution<> RandomDist(0, max);
        return RandomDist(engine);

    }
}
