#pragma once

#include <kloss/math.hpp>
#include <cstdint>
#include <vector>

struct color3ub
{
    std::uint8_t red, green, blue;
};

struct vertex
{
    color3ub color;
    vec3 position;
};

using vertex_array = std::vector<vertex>;

void draw(vertex_array const& array);
void draw_at(vertex_array const& array, vec3 const& position);

vertex_array create_grid(int size);
vertex_array create_cursor(float size);
