#pragma once

#include <array>
#include <memory>
#include <kloss/math.h>
#include "geometry.hpp"

namespace kloss {

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

Vec3 top(corner const& corner);
Vec3 bottom(corner const& corner);
void translate(corner& corner, Vec3 const& units);

using block = std::array<corner,4>;
using block_ptr = std::shared_ptr<block>;

void translate(block& block, Vec3 const& units);
std::vector<triangle> to_triangles(block const& block);

} // namespace kloss
