#pragma once

#include <kloss/group.hpp>
#include <klosscreator/group_algorithm.hpp>
#include <vector>

struct block_copy
{
    std::vector<block> blocks;
    std::vector<group> groups;
};

class block_selection
{
public:
    block_selection(group_ptr const& root);

    /**
     * Returns true if the block is selected directly or indirectly through a
     * child group.
     */
    bool is_selected(block_ref const& ref);
    /**
     * Selects the block or the group that the block is part of. The selection
     * is left unchanged if the block is part of an ancestor or unrelated group.
     */
    void select(block_ref const& ref);
    /**
     * Deselects the block or the group that the block is part of.
     */
    void deselect(block_ref const& ref);
    void deselect_all();

    void for_each_block(for_each_block_function function);

    void group();
    void ungroup();

    std::unique_ptr<block_copy> copy() const;
    /**
     * Restores an earlier copy made from this block selection. The number of
     * blocks and groups in the copy must match what is in the selection.
     */
    void restore(block_copy const& copy);
    void paste(block_copy const& copy);
    void del();

    void translate(vec3 const& units);

private:
    group_ptr root_;
    std::vector<block_ptr> blocks_;
    std::vector<group_ptr> groups_;
};

bool single_pick(block_selection& selection, block_ref const* ref);
bool multi_pick(block_selection& selection, block_ref const* ref);
