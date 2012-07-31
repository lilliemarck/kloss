#include "corner_ref.hpp"

namespace kloss {

corner_ref::corner_ref(block_ptr const& block, uint8_t corner_index)
    : block_(block)
    , corner_(&(*block)[corner_index])
    , flags_(top_flag | bottom_flag)
{
    assert(corner_index < 4);
}

corner_ref::corner_ref(corner_ref const& rhs, uint8_t flags)
    : block_(rhs.block_)
    , corner_(rhs.corner_)
    , flags_(flags)
{
}

corner const& corner_ref::operator*() const
{
    return *corner_;
}

corner& corner_ref::operator*()
{
    return *corner_;
}

corner const* corner_ref::operator->() const
{
    return corner_;
}

corner* corner_ref::operator->()
{
    return corner_;
}

corner const* corner_ref::get() const
{
    return corner_;
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

corner_ref& operator+=(corner_ref& corner_ref, cml::vector3f const& units)
{
    corner_ref->x += units[0];
    corner_ref->y += units[1];

    if (corner_ref.flags() & corner_ref::top_flag)
    {
        corner_ref->top += units[2];
    }

    if (corner_ref.flags() & corner_ref::bottom_flag)
    {
        corner_ref->bottom += units[2];
    }

    return corner_ref;
}

cml::vector3f to_vector(corner_ref const& corner_ref)
{
    bool is_top = corner_ref.flags() & corner_ref::top_flag;
    return is_top ? top(*corner_ref) : bottom(*corner_ref);
}

std::vector<corner_ref> to_corner_refs(block_ptr const& block)
{
    return
    {
        corner_ref(block, 0),
        corner_ref(block, 1),
        corner_ref(block, 2),
        corner_ref(block, 3)
    };
}

} // namespace kloss
