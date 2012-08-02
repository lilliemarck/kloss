#include "bounding_box.hpp"
#include <limits>
#include <kloss/algorithm.hpp>

namespace kloss {

bounding_box::bounding_box()
    : lower{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}
    , upper{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()}
{
}

void expand(bounding_box& lhs, bounding_box const& rhs)
{
    min(lhs.lower[0], rhs.lower[0]);
    min(lhs.lower[1], rhs.lower[1]);
    min(lhs.lower[2], rhs.lower[2]);
    max(lhs.upper[0], rhs.upper[0]);
    max(lhs.upper[1], rhs.upper[1]);
    max(lhs.upper[2], rhs.upper[2]);
}

void translate(bounding_box& bbox, cml::vector3f const& units)
{
    bbox.lower += units;
    bbox.upper += units;
}

} // namespace kloss
