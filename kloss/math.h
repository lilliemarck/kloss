#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define M_TAU   6.2831853071795864
#define M_TAU_2 3.1415926535897932
#define M_TAU_4 1.5707963267948966

typedef struct Vec2 Vec2;
typedef struct Vec3 Vec3;
typedef struct Vec4 Vec4;
typedef struct Mat4 Mat4;

struct Vec2
{
    float X, Y;
};

struct Vec3
{
    float X, Y, Z;
};

struct Vec4
{
    float X, Y, Z, W;
};

struct Mat4
{
    Vec4 X, Y, Z, T;
};

float Vec2Length(Vec2 const *vec);
float Vec2Distance(Vec2 const *lhs, Vec2 const *rhs);
float Vec2SquaredDistance(Vec2 const *lhs, Vec2 const *rhs);
float Vec2SquaredLength(Vec2 const *vec);
void Vec2Subtract(Vec2 *out, Vec2 const *lhs, Vec2 const *rhs);

void Vec3Add(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs);
void Vec3AddScaled(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs, float scale);
void Vec3Cross(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs);
float Vec3Distance(Vec3 const *lhs, Vec3 const *rhs);
float Vec3Dot(Vec3 const *lhs, Vec3 const *rhs);
bool Vec3Equals(Vec3 const *lhs, Vec3 const *rhs);
float Vec3Length(Vec3 const *vec);
void Vec3Normalize(Vec3 *out, Vec3 const *vec);
void Vec3Scale(Vec3 *out, Vec3 const *vec, float scale);
void Vec3Subtract(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs);
void Vec3Transform(Vec3 *out, Vec3 const *vec, Mat4 const *x);

void Vec4Scale(Vec4 *out, Vec4 const *vec, float scale);

void Mat4Identity(Mat4 *out);
bool Mat4Inverse(Mat4 *out, Mat4 const *mat);
void Mat4Perspective(Mat4 *out, float fovy, float aspect, float near, float far);
void Mat4RotationX(Mat4 *out, float rad);
void Mat4RotationY(Mat4 *out, float rad);
void Mat4RotationZ(Mat4 *out, float rad);
void Mat4Transform(Mat4 *out, Mat4 const *mat, Mat4 const *x);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
