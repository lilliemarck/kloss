#include "geometry.hpp"

namespace kloss {

Vec3 make_normal(triangle const& triangle)
{
    Vec3 atob, atoc, normal;
    Vec3Subtract(&atob, &triangle.b, &triangle.a);
    Vec3Subtract(&atoc, &triangle.c, &triangle.a);
    Vec3Cross(&normal, &atob, &atoc);
    Vec3Normalize(&normal, &normal);
    return normal;
}

ray make_ray_to(Vec3 const& origin, Vec3 const& target)
{
    ray ray;
    ray.origin = origin;
    Vec3Subtract(&ray.direction, &target, &origin);
    return ray;
}

boost::optional<Vec3> intersect_xy_plane(ray const& ray)
{
    return intersect_xy_plane(ray, 0.0f);
}

boost::optional<Vec3> intersect_xy_plane(ray const& ray, float z)
{
    if (ray.direction.Y != 0.0f)
    {
        float t = (z - ray.origin.Z) / ray.direction.Z;

        if (t >= 0.0f)
        {
            Vec3 pos;
            Vec3AddScaled(&pos, &ray.origin, &ray.direction, t);
            return pos;
        }
    }

    return boost::optional<Vec3>();
}

namespace {

bool same_side(struct Vec3 const *begin,
               struct Vec3 const *end,
               struct Vec3 const *point1,
               struct Vec3 const *point2)
{
    struct Vec3 dir, tmp, cp1, cp2;
    Vec3Subtract(&dir, end, begin);
    Vec3Subtract(&tmp, point1, begin);
    Vec3Cross(&cp1, &dir, &tmp);
    Vec3Subtract(&tmp, point2, begin);
    Vec3Cross(&cp2, &dir, &tmp);
    return Vec3Dot(&cp1, &cp2) >= 0.0f;
}

} // namespace

boost::optional<float> intersect(ray const& ray, triangle const& triangle)
{
    Vec3 atob, atoc, normal;
    Vec3Subtract(&atob, &triangle.b, &triangle.a);
    Vec3Subtract(&atoc, &triangle.c, &triangle.a);
    Vec3Cross(&normal, &atob, &atoc);
    float denom = Vec3Dot(&normal, &ray.direction);

    if (denom > 0.0f)
    {
        return {};
    }

    Vec3 otoa;
    Vec3Subtract(&otoa, &triangle.a, &ray.origin);
    float nom = Vec3Dot(&normal, &otoa);
    float t = nom / denom;

    if (t < 0.0f)
    {
        return {};
    }

    Vec3 point;
    Vec3AddScaled(&point, &ray.origin, &ray.direction, t);

    if (same_side(&triangle.a, &triangle.b, &point, &triangle.c) &&
        same_side(&triangle.b, &triangle.c, &point, &triangle.a) &&
        same_side(&triangle.c, &triangle.a, &point, &triangle.b))
    {
        return t;
    }

    return {};
}

} // namespace kloss
