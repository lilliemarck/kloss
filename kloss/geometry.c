#include "geometry.h"
#include <stdbool.h>
#include <stddef.h>

void TriangleNormal(Vec3 *out, Triangle const *triangle)
{
    Vec3 atob, atoc;
    Vec3Subtract(&atob, &triangle->B, &triangle->A);
    Vec3Subtract(&atoc, &triangle->C, &triangle->A);
    Vec3Cross(out, &atob, &atoc);
}

void RayFromPointToPoint(Ray *out, Vec3 const *origin, Vec3 const *target)
{
    out->Origin = *origin;
    Vec3Subtract(&out->Direction, target, origin);
}

Vec3 *RayIntersectXYPlane(Vec3 *out, Ray const *ray, float z)
{
    if (ray->Direction.Y != 0.0f)
    {
        float t = (z - ray->Origin.Z) / ray->Direction.Z;

        if (t >= 0.0f)
        {
            Vec3AddScaled(out, &ray->Origin, &ray->Direction, t);
            return out;
        }
    }

    return NULL;
}

static bool SameSide(Vec3 const *begin,
                     Vec3 const *end,
                     Vec3 const *point1,
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

float *RayIntersectTriangle(float *out, Ray const *ray, Triangle const *triangle)
{
    Vec3 normal;
    TriangleNormal(&normal, triangle);
    float denom = Vec3Dot(&normal, &ray->Direction);

    if (denom > 0.0f)
    {
        return NULL;
    }

    Vec3 otoa;
    Vec3Subtract(&otoa, &triangle->A, &ray->Origin);
    float nom = Vec3Dot(&normal, &otoa);
    float t = nom / denom;

    if (t < 0.0f)
    {
        return NULL;
    }

    Vec3 point;
    Vec3AddScaled(&point, &ray->Origin, &ray->Direction, t);

    if (SameSide(&triangle->A, &triangle->B, &point, &triangle->C) &&
        SameSide(&triangle->B, &triangle->C, &point, &triangle->A) &&
        SameSide(&triangle->C, &triangle->A, &point, &triangle->B))
    {
        *out = t;
        return out;
    }

    return NULL;
}
