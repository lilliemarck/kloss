#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>

namespace kloss {

struct ray
{
    cml::vector3f origin;
    cml::vector3f direction;
};

boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray);

} // namespace kloss
