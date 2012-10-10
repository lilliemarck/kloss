#pragma once

#include <kloss/block.hpp>
#include <vector>

struct corner_ref;

class corner_selection
{
public:
    std::size_t corner_count();
    corner_ref* get_corners();

    std::vector<corner> backup() const;
    void restore(std::vector<corner> const& backup);

private:
    std::vector<corner_ref> refs_;
    friend class corner_selector;
};

bool single_pick(corner_selection& selection, corner_ref const* ref);
bool multi_pick(corner_selection& selection, corner_ref const* ref);
