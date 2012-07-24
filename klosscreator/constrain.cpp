#include "constrain.hpp"
#include <kloss/geometry.hpp>
#include <kloss/math.hpp>

namespace kloss {
namespace creator {

boost::optional<cml::vector3f> constrain_to_z_axis(ray const& ray, cml::vector3f const& reference)
{
    /*
     * Find the z value of the intersection between the ray and a y-axis
     * aligned cylinder that is centered on the ray's origin and having a
     * radius such that the surface touches the reference point.
     */
    auto radius = distance(to_vector2(ray.origin), to_vector2(reference));
    auto denom = sqrt(square(ray.direction[0]) + square(ray.direction[1]));

    if (denom > 0.0f)
    {
        float u = radius / denom;
        return boost::optional<cml::vector3f>({reference[0], reference[1], ray.origin[2] + u * ray.direction[2]});
    }

    return {};
}

boost::optional<cml::vector3f> constrain(constrain_algorithm algorithm, ray const& ray, cml::vector3f const& reference)
{
    switch (algorithm)
    {
        case constrain_algorithm::xy_plane:
            return intersect_xy_plane(ray, reference[2]);

        case constrain_algorithm::z_axis:
            return constrain_to_z_axis(ray, reference);
    }
}

} // namespace creator
} // namespace kloss
