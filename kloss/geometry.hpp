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

boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray);
bool intersects(ray const& ray, triangle const& triangle);

} // namespace kloss
