#include "corner_ref.hpp"

namespace kloss {

corner_ref::corner_ref(block_ptr const& block, uint8_t corner_index, uint8_t flags)
    : block(block)
    , corner(&(*block)[corner_index])
    , flags(flags)
{
}

void corner_ref::translate(cml::vector3f const& units)
{
    corner->x += units[0];
    corner->y += units[1];

    if (flags & top_flag)
    {
        corner->top += units[2];
    }

    if (flags & bottom_flag)
    {
        corner->bottom += units[2];
    }
}

cml::vector3f const corner_ref::top_position() const
{
    return {corner->x, corner->y, corner->top};
}

cml::vector3f const corner_ref::bottom_position() const
{
    return {corner->x, corner->y, corner->bottom};
}

cml::vector3f to_vector(corner_ref const& corner_ref)
{
    bool top = corner_ref.flags & corner_ref::top_flag;
    float z = top ? corner_ref.corner->top : corner_ref.corner->bottom;
    return {corner_ref.corner->x, corner_ref.corner->y, z};
}

std::vector<corner_ref> to_corner_refs(block_ptr const& block)
{
    return
    {
        corner_ref(block, 0, corner_ref::top_flag),
        corner_ref(block, 1, corner_ref::top_flag),
        corner_ref(block, 2, corner_ref::top_flag),
        corner_ref(block, 3, corner_ref::top_flag),
        corner_ref(block, 0, corner_ref::bottom_flag),
        corner_ref(block, 1, corner_ref::bottom_flag),
        corner_ref(block, 2, corner_ref::bottom_flag),
        corner_ref(block, 3, corner_ref::bottom_flag)
    };
}

} // namespace kloss
