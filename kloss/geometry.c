#include "geometry.h"
#include <stdbool.h>
#include <stddef.h>

void triangle_normal(vec3 *out, triangle const *triangle)
{
    vec3 atob, atoc;
    vec3_subtract(&atob, &triangle->b, &triangle->a);
    vec3_subtract(&atoc, &triangle->c, &triangle->a);
    vec3_cross(out, &atob, &atoc);
}

void ray_from_point_to_point(ray *out, vec3 const *origin, vec3 const *target)
{
    out->origin = *origin;
    vec3_subtract(&out->direction, target, origin);
}

vec3 *ray_intersect_xyplane(vec3 *out, ray const *ray, float z)
{
    if (ray->direction.y != 0.0f)
    {
        float t = (z - ray->origin.z) / ray->direction.z;

        if (t >= 0.0f)
        {
            vec3_add_scaled(out, &ray->origin, &ray->direction, t);
            return out;
        }
    }

    return NULL;
}

static bool sameside(vec3 const *begin,
                     vec3 const *end,
                     vec3 const *point1,
                     struct vec3 const *point2)
{
    struct vec3 dir, tmp, cp1, cp2;
    vec3_subtract(&dir, end, begin);
    vec3_subtract(&tmp, point1, begin);
    vec3_cross(&cp1, &dir, &tmp);
    vec3_subtract(&tmp, point2, begin);
    vec3_cross(&cp2, &dir, &tmp);
    return vec3_dot(&cp1, &cp2) >= 0.0f;
}

float *ray_intersect_triangle(float *out, ray const *ray, triangle const *triangle)
{
    vec3 normal;
    triangle_normal(&normal, triangle);
    float denom = vec3_dot(&normal, &ray->direction);

    if (denom > 0.0f)
    {
        return NULL;
    }

    vec3 otoa;
    vec3_subtract(&otoa, &triangle->a, &ray->origin);
    float nom = vec3_dot(&normal, &otoa);
    float t = nom / denom;

    if (t < 0.0f)
    {
        return NULL;
    }

    vec3 point;
    vec3_add_scaled(&point, &ray->origin, &ray->direction, t);

    if (sameside(&triangle->a, &triangle->b, &point, &triangle->c) &&
        sameside(&triangle->b, &triangle->c, &point, &triangle->a) &&
        sameside(&triangle->c, &triangle->a, &point, &triangle->b))
    {
        *out = t;
        return out;
    }

    return NULL;
}
