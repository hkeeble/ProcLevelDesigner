#include "randomengine.h"

RandomEngine::RandomEngine()
{
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
    generator.seed(now.time_since_epoch().count());
}

int RandomEngine::randomInteger(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

float RandomEngine::randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(generator);
}
