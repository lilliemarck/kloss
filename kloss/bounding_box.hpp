#pragma once

#include <kloss/math.h>

namespace kloss {

struct bounding_box
{
    bounding_box();

    Vec3 lower;
    Vec3 upper;
};

void expand(bounding_box& lhs, bounding_box const& rhs);
void translate(bounding_box& bbox, Vec3 const& units);

} // namespace kloss
