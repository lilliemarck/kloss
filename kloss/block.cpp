#include "block.hpp"

namespace kloss {

Vec3 top(corner const& corner)
{
    return {corner.x, corner.y, corner.top};
}

Vec3 bottom(corner const& corner)
{
    return {corner.x, corner.y, corner.bottom};
}

void translate(corner& corner, Vec3 const& units)
{
    corner.x      += units.X;
    corner.y      += units.Y;
    corner.top    += units.Z;
    corner.bottom += units.Z;
}

void translate(block& block, Vec3 const& units)
{
    for (auto& corner : block)
    {
        translate(corner, units);
    }
}

std::vector<triangle> to_triangles(block const& block)
{
    return
    {
        // front
        {top(block[0]), bottom(block[0]),    top(block[1])},
        {top(block[1]), bottom(block[0]), bottom(block[1])},

        // right
        {top(block[1]), bottom(block[1]),    top(block[2])},
        {top(block[2]), bottom(block[1]), bottom(block[2])},

        // back
        {top(block[2]), bottom(block[2]),    top(block[3])},
        {top(block[3]), bottom(block[2]), bottom(block[3])},

        // left
        {top(block[3]), bottom(block[3]),    top(block[0])},
        {top(block[0]), bottom(block[3]), bottom(block[0])},

        // top
        {top(block[3]), top(block[0]), top(block[2])},
        {top(block[2]), top(block[0]), top(block[1])},

        // bottom
        {bottom(block[3]), bottom(block[2]), bottom(block[0])},
        {bottom(block[0]), bottom(block[2]), bottom(block[1])}
    };
}

} // namespace kloss
