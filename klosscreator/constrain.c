#include "constrain.h"
#include <kloss/algorithm.h>
#include <kloss/geometry.h>
#include <math.h>

static bool constrain_to_zaxis(ray const *ray, vec3 const *reference, vec3 *out)
{
    /*
     * Find the z value of the intersection between the ray and a y-axis
     * aligned cylinder that is centered on the ray's origin and having a
     * radius such that the surface touches the reference point.
     *
     * This algorithm assumes that the ray originates from the same point as
     * the camera is placed. When using a pick ray that originates from the
     * near clipping plane a small error is introduced when the camera turns
     * towards the sides.
     */
    float denom = sqrt(squaref(ray->direction.x) + squaref(ray->direction.y));

    if (denom > 0.0f)
    {
        float radius = vec2_distance((vec2 const*)&ray->origin, (vec2 const*)reference);
        float u = radius / denom;

        out->x = reference->x;
        out->y = reference->y;
        out->z = ray->origin.z + u * ray->direction.z;

        return true;
    }

    return false;
}

bool constrain_ray(constraint constraint, struct ray const *ray, struct vec3 const *reference, struct vec3 *out)
{
    switch (constraint)
    {
        case CONSTRAIN_TO_XY_PLANE:
        {
            return ray_intersect_xyplane(out, ray, reference->z);
        }

        case CONSTRAIN_TO_Z_AXIS:
        {
            return constrain_to_zaxis(ray, reference, out);
        }
    }

    return false;
}
