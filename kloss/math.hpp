#pragma once

double const tau        = 6.2831853071795864;
double const tau_over_2 = 3.1415926535897932;
double const tau_over_4 = 1.5707963267948966;

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

vec2 operator-(vec2 const& lhs, vec2 const& rhs);
float distance(vec2 const& lhs, vec2 const& rhs);
float length(vec2 const& vec);
float squared_distance(vec2 const& lhs, vec2 const& rhs);
float squared_length(vec2 const& vec);

vec3 operator*(vec3 const& vec, float scale);
vec3 operator*(float scale, vec3 const& vec);
vec3 operator*(mat4 const& lhs, vec3 const& rhs);
vec3 operator+(vec3 const& lhs, vec3 const& rhs);
vec3 operator-(vec3 const& lhs, vec3 const& rhs);
bool operator==(vec3 const& lhs, vec3 const& rhs);
bool operator!=(vec3 const& lhs, vec3 const& rhs);
vec3& operator+=(vec3& lhs, vec3 const& rhs);
vec3& operator-=(vec3& lhs, vec3 const& rhs);
vec3 cross(vec3 const& lhs, vec3 const& rhs);
float distance(vec3 const& lhs, vec3 const& rhs);
float dot(vec3 const& lhs, vec3 const& rhs);
float length(vec3 const& vec);
vec3 norm(vec3 const& vec);

vec4 operator*(vec4 const& vec, float scale);
vec4& operator*=(vec4& vec, float scale);

mat4 operator*(mat4 const& lhs, mat4 const& rhs);
mat4& operator*=(mat4& mat, float scale);
mat4 identity_matrix();
mat4 inverse(mat4 const& mat);
mat4 perspective_matrix(float fovy, float aspect, float near, float far);
mat4 rotation_matrix_x(float rad);
mat4 rotation_matrix_y(float rad);
mat4 rotation_matrix_z(float rad);

vec2 xy(vec3 const& vec);
vec3 xyz(vec4 const& vec);
