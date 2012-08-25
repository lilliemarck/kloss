#pragma once

#include <kloss/color.hpp>
#include <kloss/math.h>
#include <vector>

namespace kloss {
namespace creator {

struct vertex
{
    color3ub color;
    Vec3 position;
};

using vertex_array = std::vector<vertex>;

void draw(vertex_array const& array);
void draw(vertex_array const& array, Vec3 const& position);
void append_line(vertex_array& array, color3ub const& color, Vec3 const& begin, Vec3 const& end);

vertex_array make_grid(int size);
vertex_array make_cursor(float size);

} // namespace creator
} // namespace kloss
