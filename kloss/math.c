#include "math.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <memory.h>

float Vec2Length(Vec2 const *vec)
{
    return sqrt(vec->X * vec->X + vec->Y * vec->Y);
}

float Vec2Distance(Vec2 const *lhs, Vec2 const *rhs)
{
    Vec2 tmp;
    Vec2Subtract(&tmp, lhs, rhs);
    return Vec2Length(&tmp);
}

float Vec2SquaredDistance(Vec2 const *lhs, Vec2 const *rhs)
{
    Vec2 tmp;
    Vec2Subtract(&tmp, lhs, rhs);
    return Vec2SquaredLength(&tmp);
}

float Vec2SquaredLength(Vec2 const *vec)
{
    return vec->X * vec->X + vec->Y * vec->Y;
}

void Vec2Subtract(Vec2 *out, Vec2 const *lhs, Vec2 const *rhs)
{
    out->X = lhs->X - rhs->X;
    out->Y = lhs->Y - rhs->Y;
}

void Vec3Add(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs)
{
    out->X = lhs->X + rhs->X;
    out->Y = lhs->Y + rhs->Y;
    out->Z = lhs->Z + rhs->Z;
}

void Vec3AddScaled(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs, float scale)
{
    out->X = lhs->X + rhs->X * scale;
    out->Y = lhs->Y + rhs->Y * scale;
    out->Z = lhs->Z + rhs->Z * scale;
}

void Vec3Cross(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs)
{
    out->X = lhs->Y * rhs->Z - lhs->Z * rhs->Y;
    out->Y = lhs->Z * rhs->X - lhs->X * rhs->Z;
    out->Z = lhs->X * rhs->Y - lhs->Y * rhs->X;
}

float Vec3Distance(Vec3 const *lhs, Vec3 const *rhs)
{
    Vec3 tmp;
    Vec3Subtract(&tmp, lhs, rhs);
    return Vec3Length(&tmp);
}

float Vec3Dot(Vec3 const *lhs, Vec3 const *rhs)
{
    return lhs->X * rhs->X + lhs->Y * rhs->Y + lhs->Z * rhs->Z;
}

bool Vec3Equals(Vec3 const *lhs, Vec3 const *rhs)
{
    return lhs->X == rhs->X && lhs->Y == rhs->Y && lhs->Z == rhs->Z;
}

float Vec3Length(Vec3 const *vec)
{
    return sqrt(vec->X * vec->X + vec->Y * vec->Y + vec->Z * vec->Z);
}

void Vec3Normalize(Vec3 *out, Vec3 const *vec)
{
    float length = Vec3Length(vec);

    if (length > FLT_EPSILON)
    {
        out->X = vec->X / length;
        out->Y = vec->Y / length;
        out->Z = vec->Z / length;
    }
    else
    {
        *out = *vec;
    }
}

void Vec3Scale(Vec3 *out, Vec3 const *vec, float scale)
{
    out->X = vec->X * scale;
    out->Y = vec->Y * scale;
    out->Z = vec->Z * scale;
}

void Vec3Subtract(Vec3 *out, Vec3 const *lhs, Vec3 const *rhs)
{
    out->X = lhs->X - rhs->X;
    out->Y = lhs->Y - rhs->Y;
    out->Z = lhs->Z - rhs->Z;
}

void Vec3Transform(Vec3 *out, Vec3 const *vec, Mat4 const *x)
{
    out->X = x->X.X * vec->X + x->Y.X * vec->Y + x->Z.X * vec->Z + x->T.X;
    out->Y = x->X.Y * vec->X + x->Y.Y * vec->Y + x->Z.Y * vec->Z + x->T.Y;
    out->Z = x->X.Z * vec->X + x->Y.Z * vec->Y + x->Z.Z * vec->Z + x->T.Z;
}

void Vec4Scale(Vec4 *out, Vec4 const *vec, float scale)
{
    out->X = vec->X * scale;
    out->Y = vec->Y * scale;
    out->Z = vec->Z * scale;
    out->W = vec->W * scale;
}

void Mat4Identity(Mat4 *out)
{
    out->X.X = 1.0f;
    out->X.Y = 0.0f;
    out->X.Z = 0.0f;
    out->X.W = 0.0f;
    out->Y.X = 0.0f;
    out->Y.Y = 1.0f;
    out->Y.Z = 0.0f;
    out->Y.W = 0.0f;
    out->Z.X = 0.0f;
    out->Z.Y = 0.0f;
    out->Z.Z = 1.0f;
    out->Z.W = 0.0f;
    out->T.X = 0.0f;
    out->T.Y = 0.0f;
    out->T.Z = 0.0f;
    out->T.W = 1.0f;
}

bool Mat4Inverse(Mat4 *out, Mat4 const *mat)
{
    float tmp[12];

    /* calculate pairs for first 8 elements (cofactors) */
    tmp[ 0] = mat->Z.Z * mat->T.W;
    tmp[ 1] = mat->Z.W * mat->T.Z;
    tmp[ 2] = mat->Z.Y * mat->T.W;
    tmp[ 3] = mat->Z.W * mat->T.Y;
    tmp[ 4] = mat->Z.Y * mat->T.Z;
    tmp[ 5] = mat->Z.Z * mat->T.Y;
    tmp[ 6] = mat->Z.X * mat->T.W;
    tmp[ 7] = mat->Z.W * mat->T.X;
    tmp[ 8] = mat->Z.X * mat->T.Z;
    tmp[ 9] = mat->Z.Z * mat->T.X;
    tmp[10] = mat->Z.X * mat->T.Y;
    tmp[11] = mat->Z.Y * mat->T.X;

    /* calculate first 8 elements (cofactors) */
    out->X.X = tmp[0] * mat->Y.Y + tmp[3] * mat->Y.Z + tmp[ 4] * mat->Y.W;
    out->X.X-= tmp[1] * mat->Y.Y + tmp[2] * mat->Y.Z + tmp[ 5] * mat->Y.W;
    out->Y.X = tmp[1] * mat->Y.X + tmp[6] * mat->Y.Z + tmp[ 9] * mat->Y.W;
    out->Y.X-= tmp[0] * mat->Y.X + tmp[7] * mat->Y.Z + tmp[ 8] * mat->Y.W;
    out->Z.X = tmp[2] * mat->Y.X + tmp[7] * mat->Y.Y + tmp[10] * mat->Y.W;
    out->Z.X-= tmp[3] * mat->Y.X + tmp[6] * mat->Y.Y + tmp[11] * mat->Y.W;
    out->T.X = tmp[5] * mat->Y.X + tmp[8] * mat->Y.Y + tmp[11] * mat->Y.Z;
    out->T.X-= tmp[4] * mat->Y.X + tmp[9] * mat->Y.Y + tmp[10] * mat->Y.Z;
    out->X.Y = tmp[1] * mat->X.Y + tmp[2] * mat->X.Z + tmp[ 5] * mat->X.W;
    out->X.Y-= tmp[0] * mat->X.Y + tmp[3] * mat->X.Z + tmp[ 4] * mat->X.W;
    out->Y.Y = tmp[0] * mat->X.X + tmp[7] * mat->X.Z + tmp[ 8] * mat->X.W;
    out->Y.Y-= tmp[1] * mat->X.X + tmp[6] * mat->X.Z + tmp[ 9] * mat->X.W;
    out->Z.Y = tmp[3] * mat->X.X + tmp[6] * mat->X.Y + tmp[11] * mat->X.W;
    out->Z.Y-= tmp[2] * mat->X.X + tmp[7] * mat->X.Y + tmp[10] * mat->X.W;
    out->T.Y = tmp[4] * mat->X.X + tmp[9] * mat->X.Y + tmp[10] * mat->X.Z;
    out->T.Y-= tmp[5] * mat->X.X + tmp[8] * mat->X.Y + tmp[11] * mat->X.Z;

    /* calculate pairs for second 8 elements (cofactors) */
    tmp[ 0] = mat->X.Z * mat->Y.W;
    tmp[ 1] = mat->X.W * mat->Y.Z;
    tmp[ 2] = mat->X.Y * mat->Y.W;
    tmp[ 3] = mat->X.W * mat->Y.Y;
    tmp[ 4] = mat->X.Y * mat->Y.Z;
    tmp[ 5] = mat->X.Z * mat->Y.Y;
    tmp[ 6] = mat->X.X * mat->Y.W;
    tmp[ 7] = mat->X.W * mat->Y.X;
    tmp[ 8] = mat->X.X * mat->Y.Z;
    tmp[ 9] = mat->X.Z * mat->Y.X;
    tmp[10] = mat->X.X * mat->Y.Y;
    tmp[11] = mat->X.Y * mat->Y.X;

    /* calculate second 8 elements (cofactors) */
    out->X.Z  = tmp[ 0] * mat->T.Y + tmp[ 3] * mat->T.Z + tmp[ 4] * mat->T.W;
    out->X.Z -= tmp[ 1] * mat->T.Y + tmp[ 2] * mat->T.Z + tmp[ 5] * mat->T.W;
    out->Y.Z  = tmp[ 1] * mat->T.X + tmp[ 6] * mat->T.Z + tmp[ 9] * mat->T.W;
    out->Y.Z -= tmp[ 0] * mat->T.X + tmp[ 7] * mat->T.Z + tmp[ 8] * mat->T.W;
    out->Z.Z  = tmp[ 2] * mat->T.X + tmp[ 7] * mat->T.Y + tmp[10] * mat->T.W;
    out->Z.Z -= tmp[ 3] * mat->T.X + tmp[ 6] * mat->T.Y + tmp[11] * mat->T.W;
    out->T.Z  = tmp[ 5] * mat->T.X + tmp[ 8] * mat->T.Y + tmp[11] * mat->T.Z;
    out->T.Z -= tmp[ 4] * mat->T.X + tmp[ 9] * mat->T.Y + tmp[10] * mat->T.Z;
    out->X.W  = tmp[ 2] * mat->Z.Z + tmp[ 5] * mat->Z.W + tmp[ 1] * mat->Z.Y;
    out->X.W -= tmp[ 4] * mat->Z.W + tmp[ 0] * mat->Z.Y + tmp[ 3] * mat->Z.Z;
    out->Y.W  = tmp[ 8] * mat->Z.W + tmp[ 0] * mat->Z.X + tmp[ 7] * mat->Z.Z;
    out->Y.W -= tmp[ 6] * mat->Z.Z + tmp[ 9] * mat->Z.W + tmp[ 1] * mat->Z.X;
    out->Z.W  = tmp[ 6] * mat->Z.Y + tmp[11] * mat->Z.W + tmp[ 3] * mat->Z.X;
    out->Z.W -= tmp[10] * mat->Z.W + tmp[ 2] * mat->Z.X + tmp[ 7] * mat->Z.Y;
    out->T.W  = tmp[10] * mat->Z.Z + tmp[ 4] * mat->Z.X + tmp[ 9] * mat->Z.Y;
    out->T.W -= tmp[ 8] * mat->Z.Y + tmp[11] * mat->Z.Z + tmp[ 5] * mat->Z.X;

    // calculate determinant
    float det = mat->X.X * out->X.X + mat->X.Y * out->Y.X + mat->X.Z * out->Z.X + mat->X.W * out->T.X;

    if (fabs(det) < FLT_EPSILON)
    {
        return false;
    }

    // divide the cofactor-matrix by the determinant
    float recip = 1.0f / det;
    Vec4Scale(&out->X, &out->X, recip);
    Vec4Scale(&out->Y, &out->Y, recip);
    Vec4Scale(&out->Z, &out->Z, recip);
    Vec4Scale(&out->T, &out->T, recip);

    return true;
}

void Mat4Perspective(Mat4 *out, float fovy, float aspect, float near, float far)
{
    float f = tan(M_TAU_4 - fovy / 360.0f * M_TAU_2);

    out->X.X = f / aspect;
    out->X.Y = 0.0f;
    out->X.Z = 0.0f;
    out->X.W = 0.0f;
    out->Y.X = 0.0f;
    out->Y.Y = f;
    out->Y.Z = 0.0f;
    out->Y.W = 0.0f;
    out->Z.X = 0.0f;
    out->Z.Y = 0.0f;
    out->Z.Z = (near + far) / (near - far);
    out->Z.W = -1.0f;
    out->T.X = 0.0f;
    out->T.Y = 0.0f;
    out->T.Z = (2.0f * near * far) / (near - far);
    out->T.W = 0.0f;
}

void Mat4RotationX(Mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->X.X =  1.0f;
    out->X.Y =  0.0f;
    out->X.Z =  0.0f;
    out->X.W =  0.0f;
    out->Y.X =  0.0f;
    out->Y.Y =  cosa;
    out->Y.Z =  sina;
    out->Y.W =  0.0f;
    out->Z.X =  0.0f;
    out->Z.Y = -sina;
    out->Z.Z =  cosa;
    out->Z.W =  0.0f;
    out->T.X =  0.0f;
    out->T.Y =  0.0f;
    out->T.Z =  0.0f;
    out->T.W =  1.0f;
}

void Mat4RotationY(Mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->X.X =  cosa;
    out->X.Y =  0.0f;
    out->X.Z = -sina;
    out->X.W =  0.0f;
    out->Y.X =  0.0f;
    out->Y.Y =  1.0f;
    out->Y.Z =  0.0f;
    out->Y.W =  0.0f;
    out->Z.X =  sina;
    out->Z.Y =  0.0f;
    out->Z.Z =  cosa;
    out->Z.W =  0.0f;
    out->T.X =  0.0f;
    out->T.Y =  0.0f;
    out->T.Z =  0.0f;
    out->T.W =  1.0f;
}

void Mat4RotationZ(Mat4 *out, float rad)
{
    float cosa = cos(rad);
    float sina = sin(rad);

    out->X.X =  cosa;
    out->X.Y =  sina;
    out->X.Z =  0.0f;
    out->X.W =  0.0f;
    out->Y.X = -sina;
    out->Y.Y =  cosa;
    out->Y.Z =  0.0f;
    out->Y.W =  0.0f;
    out->Z.X =  0.0f;
    out->Z.Y =  0.0f;
    out->Z.Z =  1.0f;
    out->Z.W =  0.0f;
    out->T.X =  0.0f;
    out->T.Y =  0.0f;
    out->T.Z =  0.0f;
    out->T.W =  1.0f;
}

void Mat4Transform(Mat4 *out, Mat4 const *mat, Mat4 const *x)
{
    out->X.X = x->X.X * mat->X.X + x->Y.X * mat->X.Y + x->Z.X * mat->X.Z + x->T.X * mat->X.W;
    out->X.Y = x->X.Y * mat->X.X + x->Y.Y * mat->X.Y + x->Z.Y * mat->X.Z + x->T.Y * mat->X.W;
    out->X.Z = x->X.Z * mat->X.X + x->Y.Z * mat->X.Y + x->Z.Z * mat->X.Z + x->T.Z * mat->X.W;
    out->X.W = x->X.W * mat->X.X + x->Y.W * mat->X.Y + x->Z.W * mat->X.Z + x->T.W * mat->X.W;
    out->Y.X = x->X.X * mat->Y.X + x->Y.X * mat->Y.Y + x->Z.X * mat->Y.Z + x->T.X * mat->Y.W;
    out->Y.Y = x->X.Y * mat->Y.X + x->Y.Y * mat->Y.Y + x->Z.Y * mat->Y.Z + x->T.Y * mat->Y.W;
    out->Y.Z = x->X.Z * mat->Y.X + x->Y.Z * mat->Y.Y + x->Z.Z * mat->Y.Z + x->T.Z * mat->Y.W;
    out->Y.W = x->X.W * mat->Y.X + x->Y.W * mat->Y.Y + x->Z.W * mat->Y.Z + x->T.W * mat->Y.W;
    out->Z.X = x->X.X * mat->Z.X + x->Y.X * mat->Z.Y + x->Z.X * mat->Z.Z + x->T.X * mat->Z.W;
    out->Z.Y = x->X.Y * mat->Z.X + x->Y.Y * mat->Z.Y + x->Z.Y * mat->Z.Z + x->T.Y * mat->Z.W;
    out->Z.Z = x->X.Z * mat->Z.X + x->Y.Z * mat->Z.Y + x->Z.Z * mat->Z.Z + x->T.Z * mat->Z.W;
    out->Z.W = x->X.W * mat->Z.X + x->Y.W * mat->Z.Y + x->Z.W * mat->Z.Z + x->T.W * mat->Z.W;
    out->T.X = x->X.X * mat->T.X + x->Y.X * mat->T.Y + x->Z.X * mat->T.Z + x->T.X * mat->T.W;
    out->T.Y = x->X.Y * mat->T.X + x->Y.Y * mat->T.Y + x->Z.Y * mat->T.Z + x->T.Y * mat->T.W;
    out->T.Z = x->X.Z * mat->T.X + x->Y.Z * mat->T.Y + x->Z.Z * mat->T.Z + x->T.Z * mat->T.W;
    out->T.W = x->X.W * mat->T.X + x->Y.W * mat->T.Y + x->Z.W * mat->T.Z + x->T.W * mat->T.W;
}
