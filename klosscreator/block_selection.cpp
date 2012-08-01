#include "block_selection.hpp"
#include <kloss/algorithm.hpp>

namespace kloss {
namespace creator {

bool const block_selection::contains(block_ptr const& element) const
{
    return kloss::contains(container_, element);
}

void block_selection::insert(block_ptr const& element)
{
    container_.push_back(element);
}

void block_selection::remove(block_ptr const& element)
{
    kloss::remove(container_, element);
}

void block_selection::clear()
{
    container_.clear();
}

bool block_selection::empty() const
{
    return container_.empty();
}

block_selection::backup_type block_selection::backup() const
{
    backup_type backup;
    backup.reserve(container_.size());

    for (auto const& element : container_)
    {
        backup.push_back(*element);
    }

    return backup;
}

void block_selection::restore(backup_type const& backup)
{
    assert(backup.size() == container_.size());

    auto out_iter = container_.begin();

    for (auto const& element : backup)
    {
        **out_iter++ = element;
    }
}

std::vector<block_ptr>::iterator block_selection::begin()
{
    return container_.begin();
}

std::vector<block_ptr>::iterator block_selection::end()
{
    return container_.end();
}

} // namespace creator
} // namespace kloss
