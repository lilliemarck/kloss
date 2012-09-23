#pragma once

#include <stdbool.h>

#define M_TAU   6.2831853071795864
#define M_TAU_2 3.1415926535897932
#define M_TAU_4 1.5707963267948966

typedef struct vec2 vec2;
typedef struct vec3 vec3;
typedef struct vec4 vec4;
typedef struct mat4 mat4;

struct vec2
{
    float x, y;
};

struct vec3
{
    float x, y, z;
};

struct vec4
{
    float x, y, z, w;
};

struct mat4
{
    vec4 x, y, z, t;
};

float vec2_length(vec2 const *vec);
float vec2_distance(vec2 const *lhs, vec2 const *rhs);
float vec2_squared_distance(vec2 const *lhs, vec2 const *rhs);
float vec2_squared_length(vec2 const *vec);
void vec2_subtract(vec2 *out, vec2 const *lhs, vec2 const *rhs);

void vec3_add(vec3 *out, vec3 const *lhs, vec3 const *rhs);
void vec3_add_scaled(vec3 *out, vec3 const *lhs, vec3 const *rhs, float scale);
void vec3_cross(vec3 *out, vec3 const *lhs, vec3 const *rhs);
float vec3_distance(vec3 const *lhs, vec3 const *rhs);
float vec3_dot(vec3 const *lhs, vec3 const *rhs);
bool vec3_equals(vec3 const *lhs, vec3 const *rhs);
float vec3_length(vec3 const *vec);
void vec3_normalize(vec3 *out, vec3 const *vec);
void vec3_scale(vec3 *out, vec3 const *vec, float scale);
void vec3_subtract(vec3 *out, vec3 const *lhs, vec3 const *rhs);
void vec3_transform(vec3 *out, vec3 const *vec, mat4 const *x);

void vec4_scale(vec4 *out, vec4 const *vec, float scale);
void vec4_xyz(vec3 *out, vec4 const *vec);

void mat4_identity(mat4 *out);
bool mat4_inverse(mat4 *out, mat4 const *mat);
void mat4_perspective(mat4 *out, float fovy, float aspect, float near, float far);
void mat4_rotationx(mat4 *out, float rad);
void mat4_rotationy(mat4 *out, float rad);
void mat4_rotationz(mat4 *out, float rad);
void mat4_transform(mat4 *out, mat4 const *mat, mat4 const *x);
