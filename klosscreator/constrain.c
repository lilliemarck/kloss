#include "constrain.h"
#include <kloss/algorithm.h>
#include <kloss/geometry.h>
#include <math.h>

static bool ConstrainToZAxis(Ray const *ray, Vec3 const *reference, Vec3 *out)
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
    float denom = sqrt(Squaref(ray->Direction.X) + Squaref(ray->Direction.Y));

    if (denom > 0.0f)
    {
        float radius = Vec2Distance((Vec2 const*)&ray->Origin, (Vec2 const*)reference);
        float u = radius / denom;

        out->X = reference->X;
        out->Y = reference->Y;
        out->Z = ray->Origin.Z + u * ray->Direction.Z;

        return true;
    }

    return false;
}

bool ConstrainRay(ConstrainAlgorithm algorithm, struct Ray const *ray, struct Vec3 const *reference, struct Vec3 *out)
{
    switch (algorithm)
    {
        case CONSTRAIN_TO_XY_PLANE:
        {
            return RayIntersectXYPlane(out, ray, reference->Z);
        }

        case CONSTRAIN_TO_Z_AXIS:
        {
            return ConstrainToZAxis(ray, reference, out);
        }
    }

    return false;
}
