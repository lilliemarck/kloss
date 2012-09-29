#include "algorithm.h"
#include <math.h>
#include <stddef.h>

float clampf(float x, float min, float max)
{
    return x < min ? min : x > max ? max : x;
}

float maxf(float x, float y)
{
    return x < y ? y : x;
}

float minf(float x, float y)
{
    return x < y ? x : y;
}

int mini(int x, int y)
{
    return x < y ? x : y;
}

float roundtofractf(float x, float increment)
{
    return roundf(x / increment) * increment;
}

float squaref(float x)
{
    return x * x;
}

void *find_ptr(void *begin, void *end, void *element)
{
    for (void **i = begin; i != end; ++i)
    {
        if (*i == element)
        {
            return i;
        }
    }

    return NULL;
}
