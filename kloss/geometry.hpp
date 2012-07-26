#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>

namespace kloss {

struct triangle
{
    cml::vector3f const& operator[](std::size_t index) const { return (&a)[index]; }

    cml::vector3f a, b, c;
};

struct ray
{
    cml::vector3f origin;
    cml::vector3f direction;
};

cml::vector3f make_normal(triangle const& triangle);
ray make_ray_to(cml::vector3f const& origin, cml::vector3f const& target);
boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray);
boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray, float z);
boost::optional<float> intersect(ray const& ray, triangle const& triangle);

} // namespace kloss
