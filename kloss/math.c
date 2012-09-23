#include "math.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <memory.h>

float vec2_length(vec2 const *vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

float vec2_distance(vec2 const *lhs, vec2 const *rhs)
{
    vec2 tmp;
    vec2_subtract(&tmp, lhs, rhs);
    return vec2_length(&tmp);
}

float vec2_squared_distance(vec2 const *lhs, vec2 const *rhs)
{
    vec2 tmp;
    vec2_subtract(&tmp, lhs, rhs);
    return vec2_squared_length(&tmp);
}

float vec2_squared_length(vec2 const *vec)
{
    return vec->x * vec->x + vec->y * vec->y;
}

void vec2_subtract(vec2 *out, vec2 const *lhs, vec2 const *rhs)
{
    out->x = lhs->x - rhs->x;
    out->y = lhs->y - rhs->y;
}

void vec3_add(vec3 *out, vec3 const *lhs, vec3 const *rhs)
{
    out->x = lhs->x + rhs->x;
    out->y = lhs->y + rhs->y;
    out->z = lhs->z + rhs->z;
}

void vec3_add_scaled(vec3 *out, vec3 const *lhs, vec3 const *rhs, float scale)
{
    out->x = lhs->x + rhs->x * scale;
    out->y = lhs->y + rhs->y * scale;
    out->z = lhs->z + rhs->z * scale;
}

void vec3_cross(vec3 *out, vec3 const *lhs, vec3 const *rhs)
{
    out->x = lhs->y * rhs->z - lhs->z * rhs->y;
    out->y = lhs->z * rhs->x - lhs->x * rhs->z;
    out->z = lhs->x * rhs->y - lhs->y * rhs->x;
}

float vec3_distance(vec3 const *lhs, vec3 const *rhs)
{
    vec3 tmp;
    vec3_subtract(&tmp, lhs, rhs);
    return vec3_length(&tmp);
}

float vec3_dot(vec3 const *lhs, vec3 const *rhs)
{
    return lhs->x * rhs->x + lhs->y * rhs->y + lhs->z * rhs->z;
}

bool vec3_equals(vec3 const *lhs, vec3 const *rhs)
{
    return lhs->x == rhs->x && lhs->y == rhs->y && lhs->z == rhs->z;
}

float vec3_length(vec3 const *vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

void vec3_normalize(vec3 *out, vec3 const *vec)
{
    float length = vec3_length(vec);

    if (length > FLT_EPSILON)
    {
        out->x = vec->x / length;
        out->y = vec->y / length;
        out->z = vec->z / length;
    }
    else
    {
        *out = *vec;
    }
}

void vec3_scale(vec3 *out, vec3 const *vec, float scale)
{
    out->x = vec->x * scale;
    out->y = vec->y * scale;
    out->z = vec->z * scale;
}

void vec3_subtract(vec3 *out, vec3 const *lhs, vec3 const *rhs)
{
    out->x = lhs->x - rhs->x;
    out->y = lhs->y - rhs->y;
    out->z = lhs->z - rhs->z;
}

void vec3_transform(vec3 *out, vec3 const *vec, mat4 const *x)
{
    out->x = x->x.x * vec->x + x->y.x * vec->y + x->z.x * vec->z + x->t.x;
    out->y = x->x.y * vec->x + x->y.y * vec->y + x->z.y * vec->z + x->t.y;
    out->z = x->x.z * vec->x + x->y.z * vec->y + x->z.z * vec->z + x->t.z;
}

void vec4_scale(vec4 *out, vec4 const *vec, float scale)
{
    out->x = vec->x * scale;
    out->y = vec->y * scale;
    out->z = vec->z * scale;
    out->w = vec->w * scale;
}

void vec4_xyz(vec3 *out, vec4 const *vec)
{
    out->x = vec->x;
    out->y = vec->y;
    out->z = vec->z;
}

void mat4_identity(mat4 *out)
{
    out->x.x = 1.0f;
    out->x.y = 0.0f;
    out->x.z = 0.0f;
    out->x.w = 0.0f;
    out->y.x = 0.0f;
    out->y.y = 1.0f;
    out->y.z = 0.0f;
    out->y.w = 0.0f;
    out->z.x = 0.0f;
    out->z.y = 0.0f;
    out->z.z = 1.0f;
    out->z.w = 0.0f;
    out->t.x = 0.0f;
    out->t.y = 0.0f;
    out->t.z = 0.0f;
    out->t.w = 1.0f;
}

bool mat4_inverse(mat4 *out, mat4 const *mat)
{
    float tmp[12];

    /* calculate pairs for first 8 elements (cofactors) */
    tmp[ 0] = mat->z.z * mat->t.w;
    tmp[ 1] = mat->z.w * mat->t.z;
    tmp[ 2] = mat->z.y * mat->t.w;
    tmp[ 3] = mat->z.w * mat->t.y;
    tmp[ 4] = mat->z.y * mat->t.z;
    tmp[ 5] = mat->z.z * mat->t.y;
    tmp[ 6] = mat->z.x * mat->t.w;
    tmp[ 7] = mat->z.w * mat->t.x;
    tmp[ 8] = mat->z.x * mat->t.z;
    tmp[ 9] = mat->z.z * mat->t.x;
    tmp[10] = mat->z.x * mat->t.y;
    tmp[11] = mat->z.y * mat->t.x;

    /* calculate first 8 elements (cofactors) */
    out->x.x = tmp[0] * mat->y.y + tmp[3] * mat->y.z + tmp[ 4] * mat->y.w;
    out->x.x-= tmp[1] * mat->y.y + tmp[2] * mat->y.z + tmp[ 5] * mat->y.w;
    out->y.x = tmp[1] * mat->y.x + tmp[6] * mat->y.z + tmp[ 9] * mat->y.w;
    out->y.x-= tmp[0] * mat->y.x + tmp[7] * mat->y.z + tmp[ 8] * mat->y.w;
    out->z.x = tmp[2] * mat->y.x + tmp[7] * mat->y.y + tmp[10] * mat->y.w;
    out->z.x-= tmp[3] * mat->y.x + tmp[6] * mat->y.y + tmp[11] * mat->y.w;
    out->t.x = tmp[5] * mat->y.x + tmp[8] * mat->y.y + tmp[11] * mat->y.z;
    out->t.x-= tmp[4] * mat->y.x + tmp[9] * mat->y.y + tmp[10] * mat->y.z;
    out->x.y = tmp[1] * mat->x.y + tmp[2] * mat->x.z + tmp[ 5] * mat->x.w;
    out->x.y-= tmp[0] * mat->x.y + tmp[3] * mat->x.z + tmp[ 4] * mat->x.w;
    out->y.y = tmp[0] * mat->x.x + tmp[7] * mat->x.z + tmp[ 8] * mat->x.w;
    out->y.y-= tmp[1] * mat->x.x + tmp[6] * mat->x.z + tmp[ 9] * mat->x.w;
    out->z.y = tmp[3] * mat->x.x + tmp[6] * mat->x.y + tmp[11] * mat->x.w;
    out->z.y-= tmp[2] * mat->x.x + tmp[7] * mat->x.y + tmp[10] * mat->x.w;
    out->t.y = tmp[4] * mat->x.x + tmp[9] * mat->x.y + tmp[10] * mat->x.z;
    out->t.y-= tmp[5] * mat->x.x + tmp[8] * mat->x.y + tmp[11] * mat->x.z;

    /* calculate pairs for second 8 elements (cofactors) */
    tmp[ 0] = mat->x.z * mat->y.w;
    tmp[ 1] = mat->x.w * mat->y.z;
    tmp[ 2] = mat->x.y * mat->y.w;
    tmp[ 3] = mat->x.w * mat->y.y;
    tmp[ 4] = mat->x.y * mat->y.z;
    tmp[ 5] = mat->x.z * mat->y.y;
    tmp[ 6] = mat->x.x * mat->y.w;
    tmp[ 7] = mat->x.w * mat->y.x;
    tmp[ 8] = mat->x.x * mat->y.z;
    tmp[ 9] = mat->x.z * mat->y.x;
    tmp[10] = mat->x.x * mat->y.y;
    tmp[11] = mat->x.y * mat->y.x;

    /* calculate second 8 elements (cofactors) */
    out->x.z  = tmp[ 0] * mat->t.y + tmp[ 3] * mat->t.z + tmp[ 4] * mat->t.w;
    out->x.z -= tmp[ 1] * mat->t.y + tmp[ 2] * mat->t.z + tmp[ 5] * mat->t.w;
    out->y.z  = tmp[ 1] * mat->t.x + tmp[ 6] * mat->t.z + tmp[ 9] * mat->t.w;
    out->y.z -= tmp[ 0] * mat->t.x + tmp[ 7] * mat->t.z + tmp[ 8] * mat->t.w;
    out->z.z  = tmp[ 2] * mat->t.x + tmp[ 7] * mat->t.y + tmp[10] * mat->t.w;
    out->z.z -= tmp[ 3] * mat->t.x + tmp[ 6] * mat->t.y + tmp[11] * mat->t.w;
    out->t.z  = tmp[ 5] * mat->t.x + tmp[ 8] * mat->t.y + tmp[11] * mat->t.z;
    out->t.z -= tmp[ 4] * mat->t.x + tmp[ 9] * mat->t.y + tmp[10] * mat->t.z;
    out->x.w  = tmp[ 2] * mat->z.z + tmp[ 5] * mat->z.w + tmp[ 1] * mat->z.y;
    out->x.w -= tmp[ 4] * mat->z.w + tmp[ 0] * mat->z.y + tmp[ 3] * mat->z.z;
    out->y.w  = tmp[ 8] * mat->z.w + tmp[ 0] * mat->z.x + tmp[ 7] * mat->z.z;
    out->y.w -= tmp[ 6] * mat->z.z + tmp[ 9] * mat->z.w + tmp[ 1] * mat->z.x;
    out->z.w  = tmp[ 6] * mat->z.y + tmp[11] * mat->z.w + tmp[ 3] * mat->z.x;
    out->z.w -= tmp[10] * mat->z.w + tmp[ 2] * mat->z.x + tmp[ 7] * mat->z.y;
    out->t.w  = tmp[10] * mat->z.z + tmp[ 4] * mat->z.x + tmp[ 9] * mat->z.y;
    out->t.w -= tmp[ 8] * mat->z.y + tmp[11] * mat->z.z + tmp[ 5] * mat->z.x;

    // calculate determinant
    float det = mat->x.x * out->x.x + mat->x.y * out->y.x + mat->x.z * out->z.x + mat->x.w * out->t.x;

    if (fabs(det) < FLT_EPSILON)
    {
        return false;
    }

    // divide the cofactor-matrix by the determinant
    float recip = 1.0f / det;
    vec4_scale(&out->x, &out->x, recip);
    vec4_scale(&out->y, &out->y, recip);
    vec4_scale(&out->z, &out->z, recip);
    vec4_scale(&out->t, &out->t, recip);

    return true;
}

void mat4_perspective(mat4 *out, float fovy, float aspect, float near, float far)
{
    float f = tan(M_TAU_4 - fovy / 360.0f * M_TAU_2);

    out->x.x = f / aspect;
    out->x.y = 0.0f;
    out->x.z = 0.0f;
    out->x.w = 0.0f;
    out->y.x = 0.0f;
    out->y.y = f;
    out->y.z = 0.0f;
    out->y.w = 0.0f;
    out->z.x = 0.0f;
    out->z.y = 0.0f;
    out->z.z = (near + far) / (near - far);
    out->z.w = -1.0f;
    out->t.x = 0.0f;
    out->t.y = 0.0f;
    out->t.z = (2.0f * near * far) / (near - far);
    out->t.w = 0.0f;
}

void mat4_rotationx(mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->x.x =  1.0f;
    out->x.y =  0.0f;
    out->x.z =  0.0f;
    out->x.w =  0.0f;
    out->y.x =  0.0f;
    out->y.y =  cosa;
    out->y.z =  sina;
    out->y.w =  0.0f;
    out->z.x =  0.0f;
    out->z.y = -sina;
    out->z.z =  cosa;
    out->z.w =  0.0f;
    out->t.x =  0.0f;
    out->t.y =  0.0f;
    out->t.z =  0.0f;
    out->t.w =  1.0f;
}

void mat4_rotationy(mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->x.x =  cosa;
    out->x.y =  0.0f;
    out->x.z = -sina;
    out->x.w =  0.0f;
    out->y.x =  0.0f;
    out->y.y =  1.0f;
    out->y.z =  0.0f;
    out->y.w =  0.0f;
    out->z.x =  sina;
    out->z.y =  0.0f;
    out->z.z =  cosa;
    out->z.w =  0.0f;
    out->t.x =  0.0f;
    out->t.y =  0.0f;
    out->t.z =  0.0f;
    out->t.w =  1.0f;
}

void mat4_rotationz(mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->x.x =  cosa;
    out->x.y =  sina;
    out->x.z =  0.0f;
    out->x.w =  0.0f;
    out->y.x = -sina;
    out->y.y =  cosa;
    out->y.z =  0.0f;
    out->y.w =  0.0f;
    out->z.x =  0.0f;
    out->z.y =  0.0f;
    out->z.z =  1.0f;
    out->z.w =  0.0f;
    out->t.x =  0.0f;
    out->t.y =  0.0f;
    out->t.z =  0.0f;
    out->t.w =  1.0f;
}

void mat4_transform(mat4 *out, mat4 const *mat, mat4 const *x)
{
    out->x.x = x->x.x * mat->x.x + x->y.x * mat->x.y + x->z.x * mat->x.z + x->t.x * mat->x.w;
    out->x.y = x->x.y * mat->x.x + x->y.y * mat->x.y + x->z.y * mat->x.z + x->t.y * mat->x.w;
    out->x.z = x->x.z * mat->x.x + x->y.z * mat->x.y + x->z.z * mat->x.z + x->t.z * mat->x.w;
    out->x.w = x->x.w * mat->x.x + x->y.w * mat->x.y + x->z.w * mat->x.z + x->t.w * mat->x.w;
    out->y.x = x->x.x * mat->y.x + x->y.x * mat->y.y + x->z.x * mat->y.z + x->t.x * mat->y.w;
    out->y.y = x->x.y * mat->y.x + x->y.y * mat->y.y + x->z.y * mat->y.z + x->t.y * mat->y.w;
    out->y.z = x->x.z * mat->y.x + x->y.z * mat->y.y + x->z.z * mat->y.z + x->t.z * mat->y.w;
    out->y.w = x->x.w * mat->y.x + x->y.w * mat->y.y + x->z.w * mat->y.z + x->t.w * mat->y.w;
    out->z.x = x->x.x * mat->z.x + x->y.x * mat->z.y + x->z.x * mat->z.z + x->t.x * mat->z.w;
    out->z.y = x->x.y * mat->z.x + x->y.y * mat->z.y + x->z.y * mat->z.z + x->t.y * mat->z.w;
    out->z.z = x->x.z * mat->z.x + x->y.z * mat->z.y + x->z.z * mat->z.z + x->t.z * mat->z.w;
    out->z.w = x->x.w * mat->z.x + x->y.w * mat->z.y + x->z.w * mat->z.z + x->t.w * mat->z.w;
    out->t.x = x->x.x * mat->t.x + x->y.x * mat->t.y + x->z.x * mat->t.z + x->t.x * mat->t.w;
    out->t.y = x->x.y * mat->t.x + x->y.y * mat->t.y + x->z.y * mat->t.z + x->t.y * mat->t.w;
    out->t.z = x->x.z * mat->t.x + x->y.z * mat->t.y + x->z.z * mat->t.z + x->t.z * mat->t.w;
    out->t.w = x->x.w * mat->t.x + x->y.w * mat->t.y + x->z.w * mat->t.z + x->t.w * mat->t.w;
}
