#include "geometry.hpp"

namespace kloss {

cml::vector3f make_normal(triangle const& triangle)
{
    return normalize(cross(triangle.b - triangle.a, triangle.c - triangle.a));
}

ray make_ray_to(cml::vector3f const& origin, cml::vector3f const& target)
{
    return {origin, target - origin};
}

boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray)
{
    return intersect_xy_plane(ray, 0.0f);
}

boost::optional<cml::vector3f> intersect_xy_plane(ray const& ray, float z)
{
    if (ray.direction[1] != 0.0f)
    {
        float t = (ray.origin[2] - z) / ray.direction[2];

        if (t <= 0.0f)
        {
            return boost::optional<cml::vector3f>(ray.origin - t * ray.direction);
        }
    }

    return boost::optional<cml::vector3f>();
}

namespace {

bool same_side(cml::vector3f const& begin,
               cml::vector3f const& end,
               cml::vector3f const& point1,
               cml::vector3f const& point2)
{
    auto dir = end - begin;
    auto op1 = point1 - begin;
    auto op2 = point2 - begin;
    auto cp1 = cross(dir, op1);
    auto cp2 = cross(dir , op2);

    return dot(cp1, cp2) >= 0.0f;
}

} // namespace

boost::optional<float> intersect(ray const& ray, triangle const& triangle)
{
    auto normal = cross(triangle.b - triangle.a, triangle.c - triangle.a);
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

    auto point = ray.origin + t * ray.direction;

    if (same_side(triangle.a, triangle.b, point, triangle.c) &&
        same_side(triangle.b, triangle.c, point, triangle.a) &&
        same_side(triangle.c, triangle.a, point, triangle.b))
    {
        return t;
    }

    return {};
}

} // namespace kloss
