#include "algorithm.h"

float Clampf(float x, float min, float max)
{
    return x < min ? min : x > max ? max : x;
}

float Maxf(float x, float y)
{
    return x < y ? y : x;
}

float Minf(float x, float y)
{
    return x < y ? x : y;
}

float Squaref(float x)
{
    return x * x;
}
