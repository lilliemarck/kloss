#include "algorithm.h"
#include <math.h>

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

int Mini(int x, int y)
{
    return x < y ? x : y;
}

float RoundToStepf(float x, float increment)
{
    return roundf(x / increment) * increment;
}

float Squaref(float x)
{
    return x * x;
}
