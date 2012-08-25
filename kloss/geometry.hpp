#pragma once

#include <boost/optional.hpp>
#include <kloss/math.h>

namespace kloss {

struct triangle
{
    Vec3 const& operator[](std::size_t index) const { return (&a)[index]; }

    Vec3 a, b, c;
};

struct ray
{
    Vec3 origin;
    Vec3 direction;
};

Vec3 make_normal(triangle const& triangle);
ray make_ray_to(Vec3 const& origin, Vec3 const& target);
boost::optional<Vec3> intersect_xy_plane(ray const& ray);
boost::optional<Vec3> intersect_xy_plane(ray const& ray, float z);
boost::optional<float> intersect(ray const& ray, triangle const& triangle);

} // namespace kloss
