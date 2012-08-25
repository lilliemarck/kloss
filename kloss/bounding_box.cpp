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
    min(lhs.lower.X, rhs.lower.X);
    min(lhs.lower.Y, rhs.lower.Y);
    min(lhs.lower.Z, rhs.lower.Z);
    max(lhs.upper.X, rhs.upper.X);
    max(lhs.upper.Y, rhs.upper.Y);
    max(lhs.upper.Z, rhs.upper.Z);
}

void translate(bounding_box& bbox, Vec3 const& units)
{
    Vec3Add(&bbox.lower, &bbox.lower, &units);
    Vec3Add(&bbox.upper, &bbox.upper, &units);
}

} // namespace kloss
