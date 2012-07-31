#pragma once

#include <kloss/block.hpp>

namespace kloss {

struct corner_ref
{
    enum flags
    {
        top_flag = 1,
        bottom_flag = 2
    };

    void translate(cml::vector3f const& units);
    cml::vector3f const top_position() const;
    cml::vector3f const bottom_position() const;

    block_ptr block;
    kloss::corner* corner;
    uint8_t flags;
};

} // namespace kloss
