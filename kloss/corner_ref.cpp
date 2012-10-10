#include "corner_ref.hpp"
#include <kloss/block.hpp>
#include <cstddef>

corner_ref::corner_ref()
    : corner_(nullptr)
    , flags_(0)
{
}

corner_ref::corner_ref(corner& corner) noexcept
    : corner_(&corner)
    , flags_(corner_ref::top | corner_ref::bottom)
{
}

corner_ref::corner_ref(corner_ref const& ref, std::uint8_t flags) noexcept
    : corner_(ref.corner_)
    , flags_(flags)
{
}

corner_ref::operator bool() const noexcept
{
    return corner_;
}

corner* corner_ref::operator->() noexcept
{
    return corner_;
}

corner const* corner_ref::operator->() const noexcept
{
    return corner_;
}

corner& corner_ref::operator*() noexcept
{
    return *corner_;
}

corner const& corner_ref::operator*() const noexcept
{
    return *corner_;
}

corner* corner_ref::get() noexcept
{
    return corner_;
}

corner const* corner_ref::get() const noexcept
{
    return corner_;
}

std::uint8_t corner_ref::flags() const noexcept
{
    return flags_;
}

void corner_ref::add_flags(std::uint8_t flags) noexcept
{
    flags_ |= flags;
}

void corner_ref::clear_flags(std::uint8_t flags) noexcept
{
    flags_ &= ~flags;
}

std::array<corner_ref,4> make_corner_refs(block& block)
{
    return {{
        block.corners[0],
        block.corners[1],
        block.corners[2],
        block.corners[3]
    }};
}

void translate(corner_ref& ref, vec3 const& units)
{
    ref->x += units.x;
    ref->y += units.y;

    if (ref.flags() & corner_ref::top)
    {
        ref->top += units.z;
    }

    if (ref.flags() & corner_ref::bottom)
    {
        ref->bottom += units.z;
    }
}

vec3 get_position(corner_ref const& ref)
{
    bool is_top = ref.flags() & corner_ref::top;
    return is_top ? top(*ref) : bottom(*ref);
}
