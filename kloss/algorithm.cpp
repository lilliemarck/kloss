#include "algorithm.hpp"
#include <cmath>

float round_to_increment(float x, float increment)
{
    return std::round(x / increment) * increment;
}

float square(float x)
{
    return x * x;
}
