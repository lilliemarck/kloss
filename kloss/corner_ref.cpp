#include "corner_ref.hpp"

namespace kloss {

corner_ref::corner_ref(block_ptr const& block, uint8_t corner_index, uint8_t flags)
    : block_(block)
    , corner_(&(*block)[corner_index])
    , flags_(flags)
{
}

void corner_ref::translate(cml::vector3f const& units)
{
    corner_->x += units[0];
    corner_->y += units[1];

    if (flags_ & top_flag)
    {
        corner_->top += units[2];
    }

    if (flags_ & bottom_flag)
    {
        corner_->bottom += units[2];
    }
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

cml::vector3f const corner_ref::top_position() const
{
    return {corner_->x, corner_->y, corner_->top};
}

cml::vector3f const corner_ref::bottom_position() const
{
    return {corner_->x, corner_->y, corner_->bottom};
}

cml::vector3f to_vector(corner_ref const& corner_ref)
{
    bool top = corner_ref.flags() & corner_ref::top_flag;
    float z = top ? corner_ref.corner().top : corner_ref.corner().bottom;
    return {corner_ref.corner().x, corner_ref.corner().y, z};
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