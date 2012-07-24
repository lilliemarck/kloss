#include "block.hpp"

namespace kloss {

cml::vector3f top(corner const& corner)
{
    return {corner.x, corner.y, corner.top};
}

cml::vector3f bottom(corner const& corner)
{
    return {corner.x, corner.y, corner.bottom};
}

void translate(block& block, cml::vector3f const& units)
{
    for (auto& column : block)
    {
        column.x      += units[0];
        column.y      += units[1];
        column.top    += units[2];
        column.bottom += units[2];
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
