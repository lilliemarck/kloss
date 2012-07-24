#pragma once

namespace kloss {

template <typename T>
T square(T const& x)
{
    return x * x;
}

template <typename Vector>
float distance(Vector const& a, Vector const& b)
{
    return length(b - a);
}

inline cml::vector2f to_vector2(cml::vector3f const& v)
{
    return {v[0], v[1]};
}

} // namespace kloss
