#include "geometry.hpp"

vec3 make_normal(triangle const& triangle)
{
    return cross(triangle.b - triangle.a, triangle.c - triangle.a);
}

ray make_ray_to(vec3 const& origin, vec3 const& target)
{
    return {origin, target - origin};
}

boost::optional<vec3> intersect_xy_plane(ray const& ray, float z)
{
    if (ray.direction.y != 0.0f)
    {
        float t = (z - ray.origin.z) / ray.direction.z;

        if (t >= 0.0f)
        {
            return ray.origin + t * ray.direction;
        }
    }

    return {};
}

static bool sameside(vec3 const& begin, vec3 const& end, vec3 const& point1, vec3 const& point2)
{
    vec3 dir = end - begin;
    vec3 cp1 = cross(dir, point1 - begin);
    vec3 cp2 = cross(dir, point2 - begin);

    return dot(cp1, cp2) >= 0.0f;
}

boost::optional<float> intersect_triangle(ray const& ray, triangle const& triangle)
{
    vec3 normal = make_normal(triangle);
    float denom = dot(normal, ray.direction);

    if (denom > 0.0f)
    {
        return {};
    }

    float nom = dot(normal, triangle.a - ray.origin);
    float t = nom / denom;

    if (t < 0.0f)
    {
        return {};
    }

    vec3 point = ray.origin + t * ray.direction;

    if (sameside(triangle.a, triangle.b, point, triangle.c) &&
        sameside(triangle.b, triangle.c, point, triangle.a) &&
        sameside(triangle.c, triangle.a, point, triangle.b))
    {
        return t;
    }

    return {};
}
