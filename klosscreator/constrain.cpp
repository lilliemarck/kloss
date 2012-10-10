#include "constrain.hpp"
#include <kloss/algorithm.hpp>
#include <kloss/geometry.hpp>
#include <kloss/math.hpp>

static boost::optional<vec3> constrain_to_zaxis(ray const& ray, vec3 const& reference)
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
    float denom = sqrt(square(ray.direction.x) + square(ray.direction.y));

    if (denom > 0.0f)
    {
        float radius = distance(xy(ray.origin), xy(reference));
        float u = radius / denom;

        return {{reference.x, reference.y, ray.origin.z + u * ray.direction.z}};
    }

    return {};
}

boost::optional<vec3> constrain(constrain_mode mode, ray const& ray, vec3 const& reference)
{
    switch (mode)
    {
        case contrain_to_xy_plane:
            return intersect_xy_plane(ray, reference.z);
        case constrain_to_z_axis:
            return constrain_to_zaxis(ray, reference);
        default:
            return {};
    }
}
