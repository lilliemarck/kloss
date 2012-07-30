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

vertex_ref::vertex_ref(block_ptr const& block, uint8_t corner_index, block_side side)
    : block_(block)
    , corner_index_(corner_index)
    , side_(side)
{
    assert(block);
    assert(corner_index < 4);
}

bool const vertex_ref::operator==(vertex_ref const& rhs) const
{
    return block_ == rhs.block_ && corner_index_ == rhs.corner_index_ && side_ == rhs.side_;
}

bool const vertex_ref::operator!=(vertex_ref const& rhs) const
{
    return block_ != rhs.block_ || corner_index_ != rhs.corner_index_ || side_ != rhs.side_;
}

void vertex_ref::translate(cml::vector3f const& units)
{
    kloss::corner& corner = (*block_)[corner_index_];

    corner.x += units[0];
    corner.y += units[1];

    switch (side_)
    {
        case block_side::top:
            corner.top += units[2];
            break;

        case block_side::bottom:
            corner.bottom += units[2];
            break;
    }
}

cml::vector3f const vertex_ref::to_vector() const
{
    kloss::corner const& corner = (*block_)[corner_index_];
    float z = side_ == block_side::top ? corner.top : corner.bottom;
    return {corner.x, corner.y, z};
}

corner const& vertex_ref::corner() const
{
    return (*block_)[corner_index_];
}

corner& vertex_ref::corner()
{
    return (*block_)[corner_index_];
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

std::vector<vertex_ref> to_vertex_refs(block_ptr const& block)
{
    return
    {
        vertex_ref(block, 0, block_side::top),
        vertex_ref(block, 1, block_side::top),
        vertex_ref(block, 2, block_side::top),
        vertex_ref(block, 3, block_side::top),
        vertex_ref(block, 0, block_side::bottom),
        vertex_ref(block, 1, block_side::bottom),
        vertex_ref(block, 2, block_side::bottom),
        vertex_ref(block, 3, block_side::bottom)
    };
}

} // namespace kloss
