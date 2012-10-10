#include "block.hpp"
#include <kloss/algorithm.hpp>

block::block() : corners()
{
}

vec3 top(corner const& corner)
{
    return {corner.x, corner.y, corner.top};
}

vec3 bottom(corner const& corner)
{
    return {corner.x, corner.y, corner.bottom};
}

void translate(corner& corner, vec3 const& units)
{
    corner.x      += units.x;
    corner.y      += units.y;
    corner.top    += units.z;
    corner.bottom += units.z;
}

void translate(block& block, vec3 const& units)
{
    translate(block.corners[0], units);
    translate(block.corners[1], units);
    translate(block.corners[2], units);
    translate(block.corners[3], units);
}

void get_triangles(block const& block, std::vector<triangle>& triangles)
{
    append(triangles,
    {
        // front
        {top(block.corners[0]), bottom(block.corners[0]),    top(block.corners[1])},
        {top(block.corners[1]), bottom(block.corners[0]), bottom(block.corners[1])},

        // right
        {top(block.corners[1]), bottom(block.corners[1]),    top(block.corners[2])},
        {top(block.corners[2]), bottom(block.corners[1]), bottom(block.corners[2])},

        // back
        {top(block.corners[2]), bottom(block.corners[2]),    top(block.corners[3])},
        {top(block.corners[3]), bottom(block.corners[2]), bottom(block.corners[3])},

        // left
        {top(block.corners[3]), bottom(block.corners[3]),    top(block.corners[0])},
        {top(block.corners[0]), bottom(block.corners[3]), bottom(block.corners[0])},

        // CornerTop
        {top(block.corners[3]), top(block.corners[0]), top(block.corners[2])},
        {top(block.corners[2]), top(block.corners[0]), top(block.corners[1])},

        // bottom
        {bottom(block.corners[3]), bottom(block.corners[2]), bottom(block.corners[0])},
        {bottom(block.corners[0]), bottom(block.corners[2]), bottom(block.corners[1])}
    });
}
