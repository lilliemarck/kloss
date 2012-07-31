#include "corner_ref.hpp"

namespace kloss {

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

} // namespace kloss
