#pragma once

#include <array>
#include <vector>
#include "geometry.hpp"

namespace kloss {

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

cml::vector3f top(corner const& corner);
cml::vector3f bottom(corner const& corner);

using block = std::array<corner,4>;
using block_ptr = std::shared_ptr<block>;

std::vector<triangle> to_triangles(block const& block);

} // namespace kloss
