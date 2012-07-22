#include "geometry.hpp"

namespace kloss {

boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray)
{
    if (ray.direction[1] != 0.0f)
    {
        float t = ray.origin[2] / ray.direction[2];

        if (t <= 0.0f)
        {
            return boost::optional<cml::vector3f>(ray.origin - t * ray.direction);
        }
    }

    return boost::optional<cml::vector3f>();
}

} // namespace kloss
