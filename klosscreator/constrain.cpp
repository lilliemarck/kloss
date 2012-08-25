#include "constrain.hpp"
#include <kloss/geometry.hpp>
#include <kloss/math.hpp>

namespace kloss {
namespace creator {

static boost::optional<Vec3> constrain_to_z_axis(ray const& ray, Vec3 const& reference)
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
    auto denom = sqrt(square(ray.direction.X) + square(ray.direction.Y));

    if (denom > 0.0f)
    {
        Vec2 ray_origin_xy = to_vector2(ray.origin);
        Vec2 reference_xy = to_vector2(reference);
        auto radius = Vec2Distance(&ray_origin_xy, &reference_xy);
        float u = radius / denom;
        return boost::optional<Vec3>({reference.X, reference.Y, ray.origin.Z + u * ray.direction.Z});
    }

    return {};
}

boost::optional<Vec3> constrain(constrain_algorithm algorithm, ray const& ray, Vec3 const& reference)
{
    switch (algorithm)
    {
        case constrain_algorithm::xy_plane:
            return intersect_xy_plane(ray, reference.Z);

        case constrain_algorithm::z_axis:
            return constrain_to_z_axis(ray, reference);
    }
}

} // namespace creator
} // namespace kloss
