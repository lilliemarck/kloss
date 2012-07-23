#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>

namespace kloss {

struct triangle
{
    cml::vector3f a, b, c;
};

struct ray
{
    cml::vector3f origin;
    cml::vector3f direction;
};

cml::vector3f make_normal(triangle const& triangle);
boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray);
boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray, float z);
boost::optional<float> intersect(ray const& ray, triangle const& triangle);

} // namespace kloss
