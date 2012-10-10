#pragma once

#include <boost/optional.hpp>

struct ray;
struct vec3;

enum constrain_mode
{
    contrain_to_xy_plane,
    constrain_to_z_axis
};

/**
 * Convert a pick ray to a point by constraining it to a plane or an axis
 * around a reference point.
 */
boost::optional<vec3> constrain(constrain_mode mode, ray const& ray, vec3 const& reference);
