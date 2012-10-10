#include "document.hpp"

document::document()
    : is_locked_(false)
    , root_(new class group)
    , block_selection_(root_)
{
}

void document::lock()
{
    is_locked_ = true;
}

void document::unlock()
{
    is_locked_ = false;
}

bool document::is_locked() const
{
    return is_locked_;
}

void document::copy()
{
    if (is_locked())
    {
        return;
    }

    copied_blocks_ = block_selection_.copy();
}

void document::paste()
{
    if (is_locked() || !copied_blocks_)
    {
        return;
    }

    block_selection_.paste(*copied_blocks_);
}

void document::del()
{
    if (is_locked())
    {
        return;
    }

    block_selection_.del();
}

void document::group()
{
    if (is_locked())
    {
        return;
    }

    block_selection_.group();
}

void document::ungroup()
{
    if (is_locked())
    {
        return;
    }

    block_selection_.ungroup();
}

group_ptr document::get_root_group() const
{
    return root_;
}

block_selection& document::get_block_selection()
{
    return block_selection_;
}
