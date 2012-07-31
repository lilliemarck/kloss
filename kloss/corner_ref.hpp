#pragma once

#include <kloss/block.hpp>

namespace kloss {

class corner_ref
{
public:
    enum flags
    {
        top_flag = 1,
        bottom_flag = 2
    };

    corner_ref(block_ptr const& block, uint8_t corner_index);
    corner_ref(corner_ref const& rhs, uint8_t flags);

    corner const& operator*() const;
    corner& operator*();
    corner const* operator->() const;
    corner* operator->();
    corner const* get() const;

    uint8_t flags() const;
    void set_flags(uint8_t flags);
    void clear_flags(uint8_t flags);

private:
    block_ptr block_;
    corner* corner_;
    uint8_t flags_;
};

corner_ref& operator+=(corner_ref& corner_ref, cml::vector3f const& units);

/**
 * Return the position of either the top or bottom vertex depending on which
 * flag is set. Should not be used when both flags are set.
 */
cml::vector3f to_vector(corner_ref const& corner_ref);
std::vector<corner_ref> to_corner_refs(block_ptr const& block);

} // namespace kloss
