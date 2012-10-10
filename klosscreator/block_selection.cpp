#include "block_selection.hpp"
#include <kloss/memory.hpp>
#include <klosscreator/pick.hpp>
#include <algorithm>
#include <cassert>

block_selection::block_selection(group_ptr const& root) : root_(root)
{
}

bool block_selection::is_selected(const block_ref& ref)
{
    if (std::find(begin(blocks_), end(blocks_), ref.block) != end(blocks_))
    {
        return true;
    }

    auto const& child = get_child_by_descendant(root_, ref.group);

    return std::find(begin(groups_), end(groups_), child) != end(groups_);
}

void block_selection::select(block_ref const& ref)
{
    if (is_selected(ref))
    {
        return;
    }

    if (ref.group == root_)
    {
        blocks_.push_back(ref.block);
    }

    auto const& child = get_child_by_descendant(root_, ref.group);

    if (child)
    {
        groups_.push_back(child);
    }
}

void block_selection::deselect(block_ref const& ref)
{
    for (auto it = begin(blocks_); it != end(blocks_); ++it)
    {
        if (*it == ref.block)
        {
            blocks_.erase(it);
            return;
        }
    }

    auto const& child = get_child_by_descendant(root_, ref.group);

    for (auto it = begin(groups_); it != end(groups_); ++it)
    {
        if (*it == child)
        {
            groups_.erase(it);
            return;
        }
    }
}

void block_selection::deselect_all()
{
    blocks_.clear();
    groups_.clear();
}

void block_selection::for_each_block(for_each_block_function function)
{
    vec3 translation = root_->get_position();

    for (auto& block : blocks_)
    {
        block_ref ref = {block, root_};
        function(ref, translation);
    }

    for (auto& group : groups_)
    {
        ::for_each_block(group, translation, [&](block_ref& ref, vec3 const& pos)
        {
            function(ref, pos);
        });
    }
}

void block_selection::group()
{
    if (blocks_.empty() && groups_.empty())
    {
        return;
    }

    group_ptr new_group = std::make_shared<class group>();

    root_->remove(blocks_.data(), blocks_.size());
    new_group->insert(blocks_.data(), blocks_.size());

    for (auto& group : groups_)
    {
        root_->remove(group);
        new_group->insert(group);
    }

    deselect_all();
    root_->insert(new_group);
}

void block_selection::ungroup()
{
    for (auto& group : groups_)
    {
        root_->remove(group);
        root_->merge(group);
    }

    groups_.clear();
}

std::unique_ptr<block_copy> block_selection::copy() const
{
    auto copy = make_unique<block_copy>();

    for (auto& block : blocks_)
    {
        copy->blocks.push_back(*block);
    }

    for (auto& group : groups_)
    {
        copy->groups.push_back(*group);
    }

    return copy;
}

void block_selection::restore(block_copy const& copy)
{
    assert(copy.blocks.size() == blocks_.size());
    assert(copy.groups.size() == groups_.size());

    std::size_t blockcount = blocks_.size();

    for (std::size_t i = 0; i < blockcount; ++i)
    {
        *blocks_[i] = copy.blocks[i];
    }

    std::size_t groupcount = groups_.size();

    for (std::size_t i = 0; i < groupcount; ++i)
    {
        vec3 pos = copy.groups[i].get_position();
        groups_[i]->set_position(pos);
    }
}

void block_selection::paste(const block_copy& copy)
{
    deselect_all();

    for (auto& block : copy.blocks)
    {
        block_ptr copy = std::make_shared<struct block>(block);
        root_->insert(&copy, 1);
        blocks_.push_back(copy);
    }

    for (auto& group : copy.groups)
    {
        group_ptr copy = make_unique<class group>(group);
        root_->insert(copy);
        groups_.push_back(copy);
    }
}

void block_selection::del()
{
    root_->remove(blocks_.data(), blocks_.size());

    for (auto& group : groups_)
    {
        root_->remove(group);
    }

    deselect_all();
}

void block_selection::translate(const vec3& units)
{
    for (auto& block : blocks_)
    {
        ::translate(*block, units);
    }

    for (auto& group : groups_)
    {
        group->set_position(group->get_position() + units);
    }

    root_->update_vertex_array();
}

class block_selector : public selector
{
public:
    block_selector(block_selection& selection, block_ref const& ref)
        : selection_(selection)
        , ref_(ref)
    {}

    bool is_selected() override
    {
        return selection_.is_selected(ref_);
    }

    void select() override
    {
        selection_.select(ref_);
    }

    void deselect() override
    {
        selection_.deselect(ref_);
    }

    void deselect_all() override
    {
        selection_.deselect_all();
    }

private:
    block_selection& selection_;
    block_ref const& ref_;
};

bool single_pick(block_selection& selection, block_ref const* ref)
{
    block_selector selector(selection, *ref);
    return single_pick(selector, ref);
}

bool multi_pick(block_selection& selection, block_ref const* ref)
{
    block_selector selector(selection, *ref);
    return multi_pick(selector, ref);
}
