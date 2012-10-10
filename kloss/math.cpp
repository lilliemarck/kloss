#include "math.hpp"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstring>

vec2 operator-(vec2 const& lhs, vec2 const& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

float distance(vec2 const& lhs, vec2 const& rhs)
{
    return length(lhs - rhs);
}

float length(vec2 const& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float squared_distance(vec2 const& lhs, vec2 const& rhs)
{
    return squared_length(lhs - rhs);
}

float squared_length(vec2 const& vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

vec3 operator*(vec3 const& vec, float scale)
{
    return {vec.x * scale, vec.y * scale, vec.z * scale};
}

vec3 operator*(float scale, vec3 const& vec)
{
    return {scale * vec.x, scale * vec.y, scale * vec.z};
}

vec3 operator*(mat4 const& lhs, vec3 const& rhs)
{
    return {lhs.x.x * rhs.x + lhs.y.x * rhs.y + lhs.z.x * rhs.z + lhs.t.x,
            lhs.x.y * rhs.x + lhs.y.y * rhs.y + lhs.z.y * rhs.z + lhs.t.y,
            lhs.x.z * rhs.x + lhs.y.z * rhs.y + lhs.z.z * rhs.z + lhs.t.z};
}

vec3 operator+(vec3 const& lhs, vec3 const& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

vec3 operator-(vec3 const& lhs, vec3 const& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

bool operator==(vec3 const& lhs, vec3 const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(vec3 const& lhs, vec3 const& rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

vec3& operator+=(vec3& lhs, vec3 const& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;

    return lhs;
}

vec3& operator-=(vec3& lhs, vec3 const& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;

    return lhs;
}

vec3 cross(vec3 const& lhs, vec3 const& rhs)
{
    return {lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x};
}

float distance(vec3 const& lhs, vec3 const& rhs)
{
    return length(lhs - rhs);
}

float dot(vec3 const& lhs, vec3 const& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float length(vec3 const& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

vec3 norm(vec3 const& vec)
{
    float length = ::length(vec);

    if (length > FLT_EPSILON)
    {
        return {vec.x / length, vec.y / length, vec.z / length};
    }
    else
    {
        return vec;
    }
}

vec4 operator*(vec4 const& vec, float scale)
{
    return {vec.x * scale, vec.y * scale, vec.z * scale, vec.w * scale};
}

vec4& operator*=(vec4& vec, float scale)
{
    vec.x *= scale;
    vec.y *= scale;
    vec.z *= scale;
    vec.w *= scale;

    return vec;
}

mat4 operator*(mat4 const& lhs, mat4 const& rhs)
{
    return
    {
        {lhs.x.x * rhs.x.x + lhs.y.x * rhs.x.y + lhs.z.x * rhs.x.z + lhs.t.x * rhs.x.w,
         lhs.x.y * rhs.x.x + lhs.y.y * rhs.x.y + lhs.z.y * rhs.x.z + lhs.t.y * rhs.x.w,
         lhs.x.z * rhs.x.x + lhs.y.z * rhs.x.y + lhs.z.z * rhs.x.z + lhs.t.z * rhs.x.w,
         lhs.x.w * rhs.x.x + lhs.y.w * rhs.x.y + lhs.z.w * rhs.x.z + lhs.t.w * rhs.x.w},
        {lhs.x.x * rhs.y.x + lhs.y.x * rhs.y.y + lhs.z.x * rhs.y.z + lhs.t.x * rhs.y.w,
         lhs.x.y * rhs.y.x + lhs.y.y * rhs.y.y + lhs.z.y * rhs.y.z + lhs.t.y * rhs.y.w,
         lhs.x.z * rhs.y.x + lhs.y.z * rhs.y.y + lhs.z.z * rhs.y.z + lhs.t.z * rhs.y.w,
         lhs.x.w * rhs.y.x + lhs.y.w * rhs.y.y + lhs.z.w * rhs.y.z + lhs.t.w * rhs.y.w},
        {lhs.x.x * rhs.z.x + lhs.y.x * rhs.z.y + lhs.z.x * rhs.z.z + lhs.t.x * rhs.z.w,
         lhs.x.y * rhs.z.x + lhs.y.y * rhs.z.y + lhs.z.y * rhs.z.z + lhs.t.y * rhs.z.w,
         lhs.x.z * rhs.z.x + lhs.y.z * rhs.z.y + lhs.z.z * rhs.z.z + lhs.t.z * rhs.z.w,
         lhs.x.w * rhs.z.x + lhs.y.w * rhs.z.y + lhs.z.w * rhs.z.z + lhs.t.w * rhs.z.w},
        {lhs.x.x * rhs.t.x + lhs.y.x * rhs.t.y + lhs.z.x * rhs.t.z + lhs.t.x * rhs.t.w,
         lhs.x.y * rhs.t.x + lhs.y.y * rhs.t.y + lhs.z.y * rhs.t.z + lhs.t.y * rhs.t.w,
         lhs.x.z * rhs.t.x + lhs.y.z * rhs.t.y + lhs.z.z * rhs.t.z + lhs.t.z * rhs.t.w,
         lhs.x.w * rhs.t.x + lhs.y.w * rhs.t.y + lhs.z.w * rhs.t.z + lhs.t.w * rhs.t.w}
    };
}

mat4& operator*=(mat4& mat, float scale)
{
    mat.x *= scale;
    mat.y *= scale;
    mat.z *= scale;
    mat.t *= scale;

    return mat;
}


mat4 identity_matrix()
{
    return {{1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

mat4 inverse(mat4 const& mat)
{
    mat4 out;
    float tmp[12];

    /* calculate pairs for first 8 elements (cofactors) */
    tmp[ 0] = mat.z.z * mat.t.w;
    tmp[ 1] = mat.z.w * mat.t.z;
    tmp[ 2] = mat.z.y * mat.t.w;
    tmp[ 3] = mat.z.w * mat.t.y;
    tmp[ 4] = mat.z.y * mat.t.z;
    tmp[ 5] = mat.z.z * mat.t.y;
    tmp[ 6] = mat.z.x * mat.t.w;
    tmp[ 7] = mat.z.w * mat.t.x;
    tmp[ 8] = mat.z.x * mat.t.z;
    tmp[ 9] = mat.z.z * mat.t.x;
    tmp[10] = mat.z.x * mat.t.y;
    tmp[11] = mat.z.y * mat.t.x;

    /* calculate first 8 elements (cofactors) */
    out.x.x = tmp[0] * mat.y.y + tmp[3] * mat.y.z + tmp[ 4] * mat.y.w;
    out.x.x-= tmp[1] * mat.y.y + tmp[2] * mat.y.z + tmp[ 5] * mat.y.w;
    out.y.x = tmp[1] * mat.y.x + tmp[6] * mat.y.z + tmp[ 9] * mat.y.w;
    out.y.x-= tmp[0] * mat.y.x + tmp[7] * mat.y.z + tmp[ 8] * mat.y.w;
    out.z.x = tmp[2] * mat.y.x + tmp[7] * mat.y.y + tmp[10] * mat.y.w;
    out.z.x-= tmp[3] * mat.y.x + tmp[6] * mat.y.y + tmp[11] * mat.y.w;
    out.t.x = tmp[5] * mat.y.x + tmp[8] * mat.y.y + tmp[11] * mat.y.z;
    out.t.x-= tmp[4] * mat.y.x + tmp[9] * mat.y.y + tmp[10] * mat.y.z;
    out.x.y = tmp[1] * mat.x.y + tmp[2] * mat.x.z + tmp[ 5] * mat.x.w;
    out.x.y-= tmp[0] * mat.x.y + tmp[3] * mat.x.z + tmp[ 4] * mat.x.w;
    out.y.y = tmp[0] * mat.x.x + tmp[7] * mat.x.z + tmp[ 8] * mat.x.w;
    out.y.y-= tmp[1] * mat.x.x + tmp[6] * mat.x.z + tmp[ 9] * mat.x.w;
    out.z.y = tmp[3] * mat.x.x + tmp[6] * mat.x.y + tmp[11] * mat.x.w;
    out.z.y-= tmp[2] * mat.x.x + tmp[7] * mat.x.y + tmp[10] * mat.x.w;
    out.t.y = tmp[4] * mat.x.x + tmp[9] * mat.x.y + tmp[10] * mat.x.z;
    out.t.y-= tmp[5] * mat.x.x + tmp[8] * mat.x.y + tmp[11] * mat.x.z;

    /* calculate pairs for second 8 elements (cofactors) */
    tmp[ 0] = mat.x.z * mat.y.w;
    tmp[ 1] = mat.x.w * mat.y.z;
    tmp[ 2] = mat.x.y * mat.y.w;
    tmp[ 3] = mat.x.w * mat.y.y;
    tmp[ 4] = mat.x.y * mat.y.z;
    tmp[ 5] = mat.x.z * mat.y.y;
    tmp[ 6] = mat.x.x * mat.y.w;
    tmp[ 7] = mat.x.w * mat.y.x;
    tmp[ 8] = mat.x.x * mat.y.z;
    tmp[ 9] = mat.x.z * mat.y.x;
    tmp[10] = mat.x.x * mat.y.y;
    tmp[11] = mat.x.y * mat.y.x;

    /* calculate second 8 elements (cofactors) */
    out.x.z  = tmp[ 0] * mat.t.y + tmp[ 3] * mat.t.z + tmp[ 4] * mat.t.w;
    out.x.z -= tmp[ 1] * mat.t.y + tmp[ 2] * mat.t.z + tmp[ 5] * mat.t.w;
    out.y.z  = tmp[ 1] * mat.t.x + tmp[ 6] * mat.t.z + tmp[ 9] * mat.t.w;
    out.y.z -= tmp[ 0] * mat.t.x + tmp[ 7] * mat.t.z + tmp[ 8] * mat.t.w;
    out.z.z  = tmp[ 2] * mat.t.x + tmp[ 7] * mat.t.y + tmp[10] * mat.t.w;
    out.z.z -= tmp[ 3] * mat.t.x + tmp[ 6] * mat.t.y + tmp[11] * mat.t.w;
    out.t.z  = tmp[ 5] * mat.t.x + tmp[ 8] * mat.t.y + tmp[11] * mat.t.z;
    out.t.z -= tmp[ 4] * mat.t.x + tmp[ 9] * mat.t.y + tmp[10] * mat.t.z;
    out.x.w  = tmp[ 2] * mat.z.z + tmp[ 5] * mat.z.w + tmp[ 1] * mat.z.y;
    out.x.w -= tmp[ 4] * mat.z.w + tmp[ 0] * mat.z.y + tmp[ 3] * mat.z.z;
    out.y.w  = tmp[ 8] * mat.z.w + tmp[ 0] * mat.z.x + tmp[ 7] * mat.z.z;
    out.y.w -= tmp[ 6] * mat.z.z + tmp[ 9] * mat.z.w + tmp[ 1] * mat.z.x;
    out.z.w  = tmp[ 6] * mat.z.y + tmp[11] * mat.z.w + tmp[ 3] * mat.z.x;
    out.z.w -= tmp[10] * mat.z.w + tmp[ 2] * mat.z.x + tmp[ 7] * mat.z.y;
    out.t.w  = tmp[10] * mat.z.z + tmp[ 4] * mat.z.x + tmp[ 9] * mat.z.y;
    out.t.w -= tmp[ 8] * mat.z.y + tmp[11] * mat.z.z + tmp[ 5] * mat.z.x;

    // calculate determinant
    float det = mat.x.x * out.x.x + mat.x.y * out.y.x + mat.x.z * out.z.x + mat.x.w * out.t.x;

    // divide the cofactor-matrix by the determinant
    out *= 1.0f / det;

    return out;
}

mat4 perspective_matrix(float fovy, float aspect, float near, float far)
{
    float f = tan(tau_over_4 - fovy / 360.0f * tau_over_2);

    return {{f / aspect, 0.0f, 0.0f, 0.0f},
            {0.0f, f, 0.0f, 0.0f},
            {0.0f, 0.0f, (near + far) / (near - far), -1.0f},
            {0.0f, 0.0f, (2.0f * near * far) / (near - far), 0.0f}};
}

mat4 rotation_matrix_x(float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    return {{1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosa, sina, 0.0f},
            {0.0f,-sina, cosa, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

mat4 rotation_matrix_y(float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    return {{cosa, 0.0f,-sina, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {sina, 0.0f, cosa, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}};
}

mat4 rotation_matrix_z(float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    return {{ cosa, sina, 0.0f, 0.0f},
            {-sina, cosa, 0.0f, 0.0f},
            { 0.0f, 0.0f, 1.0f, 0.0f},
            { 0.0f, 0.0f, 0.0f, 1.0f}};
}

vec2 xy(vec3 const& vec)
{
    return {vec.x, vec.y};
}

vec3 xyz(vec4 const& vec)
{
    return {vec.x, vec.y, vec.z};
}
