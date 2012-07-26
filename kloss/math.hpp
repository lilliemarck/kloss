#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>

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

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

boost::optional<cml::vector3f> project(cml::vector3f const& point,
                                       cml::matrix44f_c const& model,
                                       cml::matrix44f_c const& projection,
                                       viewport const& viewport);

} // namespace kloss
