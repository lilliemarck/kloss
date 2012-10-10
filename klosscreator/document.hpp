#pragma once

#include <kloss/group.hpp>
#include <klosscreator/block_selection.hpp>

class document
{
public:
    document();

    void lock();
    void unlock();
    bool is_locked() const;

    void copy();
    void paste();
    void del();
    void group();
    void ungroup();

    group_ptr get_root_group() const;
    block_selection& get_block_selection();

private:
    bool is_locked_;
    group_ptr root_;
    block_selection block_selection_;
    std::unique_ptr<block_copy> copied_blocks_;
};
