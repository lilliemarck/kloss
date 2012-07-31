#include "corner_ref.hpp"

namespace kloss {

corner_ref::corner_ref(block_ptr const& block, uint8_t corner_index, uint8_t flags)
    : block_(block)
    , corner_(&(*block)[corner_index])
    , flags_(flags)
{
}

corner const& corner_ref::corner() const
{
    return *corner_;
}

corner& corner_ref::corner()
{
    return *corner_;
}

uint8_t corner_ref::flags() const
{
    return flags_;
}

void corner_ref::set_flags(uint8_t flags)
{
    flags_ |= flags;
}

void corner_ref::clear_flags(uint8_t flags)
{
    flags_ &= ~flags;
}

cml::vector3f top(corner_ref const& corner_ref)
{
    corner const& corner = corner_ref.corner();
    return {corner.x, corner.y, corner.top};
}

cml::vector3f bottom(corner_ref const& corner_ref)
{
    corner const& corner = corner_ref.corner();
    return {corner.x, corner.y, corner.bottom};
}

corner_ref& operator+=(corner_ref& corner_ref, cml::vector3f const& units)
{
    corner& corner = corner_ref.corner();

    corner.x += units[0];
    corner.y += units[1];

    if (corner_ref.flags() & corner_ref::top_flag)
    {
        corner.top += units[2];
    }

    if (corner_ref.flags() & corner_ref::bottom_flag)
    {
        corner.bottom += units[2];
    }

    return corner_ref;
}

cml::vector3f to_vector(corner_ref const& corner_ref)
{
    bool is_top = corner_ref.flags() & corner_ref::top_flag;
    return is_top ? top(corner_ref) : bottom(corner_ref);
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
