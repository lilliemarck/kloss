#include "document.hpp"

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

} // namespace creator
} // namespace kloss
