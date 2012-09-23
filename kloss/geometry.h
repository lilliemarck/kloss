#pragma once

#include <kloss/math.h>

typedef struct triangle triangle;
typedef struct ray ray;

struct triangle
{
    vec3 a, b, c;
};

struct ray
{
    vec3 origin;
    vec3 direction;
};

void triangle_normal(vec3 *out, triangle const *triangle);
void ray_from_point_to_point(ray *out, vec3 const *origin, vec3 const *target);
vec3 *ray_intersect_xyplane(vec3 *out, ray const *ray, float z);
float *ray_intersect_triangle(float *out, ray const *ray, triangle const *triangle);
