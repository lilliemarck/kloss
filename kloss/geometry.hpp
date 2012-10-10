#pragma once

#include <boost/optional.hpp>
#include <kloss/math.hpp>

struct triangle
{
    vec3 a, b, c;
};

struct ray
{
    vec3 origin;
    vec3 direction;
};

vec3 make_normal(triangle const& triangle);
ray make_ray_to(vec3 const& origin, vec3 const& target);
boost::optional<vec3> intersect_xy_plane(ray const& ray, float z);
boost::optional<float> intersect_triangle(ray const& ray, triangle const& triangle);
