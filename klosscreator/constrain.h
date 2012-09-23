#pragma once

#include <stdbool.h>

struct ray;
struct vec3;

enum constraint
{
    CONSTRAIN_TO_XY_PLANE,
    CONSTRAIN_TO_Z_AXIS
};

typedef enum constraint constraint;

/**
 * Convert a pick ray to a point by constraining it to a plane or an axis
 * around a reference point.
 */
bool constrain_ray(constraint constraint, struct ray const *ray, struct vec3 const *reference, struct vec3 *out);
