#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <kloss/math.h>

namespace kloss {

template <typename T>
T const round(T const& x, float increment)
{
    return std::round(x / increment) * increment;
}

template <typename T>
T const square(T const& x)
{
    return x * x;
}

inline Vec2 const to_vector2(Vec3 const& v)
{
    return {v.X, v.Y};
}

} // namespace kloss
