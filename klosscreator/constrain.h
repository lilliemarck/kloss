#pragma once

#include <stdbool.h>

struct Ray;
struct Vec3;

enum ConstrainAlgorithm
{
    CONSTRAIN_TO_XY_PLANE,
    CONSTRAIN_TO_Z_AXIS
};

typedef enum ConstrainAlgorithm ConstrainAlgorithm;

/**
 * Convert a pick ray to a point by constraining it to a plane or an axis
 * around a reference point.
 */
bool ConstrainRay(ConstrainAlgorithm algorithm, struct Ray const *ray, struct Vec3 const *reference, struct Vec3 *out);
