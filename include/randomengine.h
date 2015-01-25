#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include <random>
#include <chrono>

/**
 * @brief A wrapper around a C++11 random engine for improved usability.
 */
class RandomEngine
{
public:
    RandomEngine();

    /*!
     * \brief randomInteger Returns a random integer.
     * \param min The lower, inclusive bound of the random value.
     * \param max The upper, exclusive, bound of the random value.
     * \return A random integer between min and max.
     */
    int randomInteger(int min, int max);

    /*!
     * \brief randomFloat Returns a floating-point value.
     * \param min The lower, inclusive bound of the random value.
     * \param max The upper, exclusive, bound of the random value.
     * \return A random floating-point value between min and max.
     */
    float randomFloat(float min, float max);

private:
   std::default_random_engine generator; /*!< The random generator. */
};

#endif // RANDOMENGINE_H
