#pragma once

#include <boost/optional.hpp>
#include <kloss/geometry.hpp>

namespace kloss {
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
boost::optional<Vec3> constrain(constrain_algorithm algorithm, ray const& ray, Vec3 const& reference);

} // namespace creator
} // namespace kloss
