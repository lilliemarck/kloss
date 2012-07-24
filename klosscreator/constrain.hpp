#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>

namespace kloss {

struct ray;

namespace creator {

enum class constrain_algorithm
{
    xy_plane,
    z_axis
};

/**
 * Convert a pick ray to a point by constraining it to a plane or an axis
 * around a reference point.
 */
boost::optional<cml::vector3f> constrain(constrain_algorithm algorithm, ray const& ray, cml::vector3f const& reference);

} // namespace creator
} // namespace kloss
