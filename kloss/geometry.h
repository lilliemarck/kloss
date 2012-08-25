#pragma once

#include <kloss/math.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Triangle Triangle;
typedef struct Ray Ray;

struct Triangle
{
    Vec3 A, B, C;
};

struct Ray
{
    Vec3 Origin;
    Vec3 Direction;
};

void TriangleNormal(Vec3 *out, Triangle const *triangle);
void RayFromPointToPoint(Ray *out, Vec3 const *origin, Vec3 const *target);
Vec3 *RayIntersectXYPlane(Vec3 *out, Ray const *ray, float z);
float *RayIntersectTriangle(float *out, Ray const *ray, Triangle const *triangle);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
