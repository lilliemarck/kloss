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

    corner_ref(block_ptr const& block, uint8_t corner_index, uint8_t flags);
    void translate(cml::vector3f const& units);
    cml::vector3f const top_position() const;
    cml::vector3f const bottom_position() const;

    block_ptr block;
    kloss::corner* corner;
    uint8_t flags;
};

/**
 * Return the position of either the top or bottom vertex depending on which
 * flag is set. Should not be used when both flags are set.
 */
cml::vector3f to_vector(corner_ref const& corner_ref);
std::vector<corner_ref> to_corner_refs(block_ptr const& block);

} // namespace kloss
