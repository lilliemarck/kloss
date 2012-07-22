#pragma once

#include <cml/cml.h>
#include <kloss/color.hpp>

namespace kloss {
namespace creator {

struct vertex
{
    color3ub color;
    cml::vector3f position;
};

using vertex_array = std::vector<vertex>;

void draw(vertex_array const& array);
void draw(vertex_array const& array, cml::vector3f const& position);
void append_line(vertex_array& array, color3ub const& color, cml::vector3f const& begin, cml::vector3f const& end);

vertex_array make_grid(int size);
vertex_array make_cursor(float size);

} // namespace creator
} // namespace kloss
