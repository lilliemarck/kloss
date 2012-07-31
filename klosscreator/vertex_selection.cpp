#include "vertex_selection.hpp"
#include <kloss/algorithm.hpp>

namespace kloss {
namespace creator {

bool const vertex_selection::contains(corner_ref const& element) const
{
    for (auto const& corner_ref : container_)
    {
        if (&corner_ref.corner() == &element.corner() &&
            corner_ref.flags() & element.flags())
        {
            return true;
        }
    }

    return false;
}

void vertex_selection::insert(corner_ref& element)
{
    for (auto& corner_ref : container_)
    {
        if (&corner_ref.corner() == &element.corner())
        {
            corner_ref.set_flags(element.flags());
            return;
        }
    }

    container_.push_back(element);
}

void vertex_selection::remove(corner_ref const& element)
{
    for (auto iter = container_.begin(); iter != container_.end(); ++iter)
    {
        auto& corner_ref = *iter;

        if (&corner_ref.corner() == &element.corner())
        {
            corner_ref.clear_flags(element.flags());

            if (corner_ref.flags() == 0)
            {
                container_.erase(iter);
                return;
            }
        }
    }
}

void vertex_selection::clear()
{
    container_.clear();
}

vertex_selection::backup_type vertex_selection::backup() const
{
    backup_type backup;
    backup.reserve(container_.size());

    for (auto const& corner_ref : container_)
    {
        backup.push_back(corner_ref.corner());
    }

    return backup;
}

void vertex_selection::restore(backup_type const& backup)
{
    assert(backup.size() == container_.size());

    auto out_iter = container_.begin();

    for (auto const& element : backup)
    {
        (out_iter++)->corner() = element;
    }
}

std::vector<corner_ref>::iterator vertex_selection::begin()
{
    return container_.begin();
}

std::vector<corner_ref>::iterator vertex_selection::end()
{
    return container_.end();
}

} // namespace creator
} // namespace kloss
