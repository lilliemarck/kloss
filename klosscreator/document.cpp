#include "document.hpp"
#include <kloss/group_instance.hpp>

namespace kloss {
namespace creator {

document::document() : lock_count_(0)
{
}

document::~document()
{
    assert(!is_locked());
}

void document::lock() noexcept
{
    ++lock_count_;
}

void document::unlock() noexcept
{
    assert(is_locked());
    --lock_count_;
}

bool document::is_locked() const
{
    return lock_count_;
}

void document::cut()
{
    if (is_locked() || block_selection.empty())
    {
        return;
    }

    copied_blocks_ = block_selection.backup();
    del();
}

void document::copy()
{
    if (is_locked() || block_selection.empty())
    {
        return;
    }

    copied_blocks_ = block_selection.backup();
}

void document::paste()
{
    if (is_locked() || copied_blocks_.empty())
    {
        return;
    }

    block_selection.clear();

    for (auto const& block : copied_blocks_)
    {
        auto new_block = std::make_shared<kloss::block>(block);
        group.insert(new_block);
        block_selection.insert(new_block);
    }
}

void document::del()
{
    if (is_locked() || block_selection.empty())
    {
        return;
    }

    for (auto block : block_selection)
    {
        group.remove(block);
    }

    block_selection.clear();
    group.update_vertex_array();
}

void document::group_selection()
{
    if (is_locked() || block_selection.empty())
    {
        return;
    }

    for (auto block : block_selection)
    {
        group.remove(block);
    }

    group_ptr new_group = std::make_shared<kloss::group>();

    for (auto block : block_selection)
    {
        new_group->insert(block);
    }

    block_selection.clear();

    group.update_vertex_array();
    new_group->update_vertex_array();

    group.insert(group_instance(new_group));
}

} // namespace creator
} // namespace kloss
