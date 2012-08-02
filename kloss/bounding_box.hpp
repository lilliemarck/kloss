#pragma once

#include <cml/cml.h>

namespace kloss {

struct bounding_box
{
    bounding_box();

    cml::vector3f lower;
    cml::vector3f upper;
};

void expand(bounding_box& lhs, bounding_box const& rhs);
void translate(bounding_box& bbox, cml::vector3f const& units);

} // namespace kloss
