#include "vertex_selection.hpp"
#include <kloss/algorithm.hpp>

namespace kloss {
namespace creator {
namespace {

uint8_t side_to_flag(vertex_ref const& vertex)
{
    switch (vertex.side())
    {
        case block_side::top:
            return corner_ref::top_flag;

        case block_side::bottom:
            return corner_ref::bottom_flag;
    }
}

} // namespace

bool const vertex_selection::contains(vertex_ref const& element) const
{
    auto const flag = side_to_flag(element);

    for (auto const& e : container_)
    {
        if (e.corner == &element.corner() && e.flags & flag)
        {
            return true;
        }
    }

    return false;
}

void vertex_selection::insert(vertex_ref& element)
{
    auto const flag = side_to_flag(element);

    for (auto& e : container_)
    {
        if (e.corner == &element.corner())
        {
            e.flags |= flag;
            return;
        }
    }

    container_.push_back(corner_ref{element.block(), &element.corner(), flag});
}

void vertex_selection::remove(vertex_ref const& element)
{
    auto const flag = side_to_flag(element);

    for (auto iter = container_.begin(); iter != container_.end(); ++iter)
    {
        auto& e = *iter;

        if (e.corner == &element.corner())
        {
            e.flags &= ~flag;

            if (e.flags == 0)
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

    for (auto const& element : container_)
    {
        backup.push_back(*element.corner);
    }

    return backup;
}

void vertex_selection::restore(backup_type const& backup)
{
    assert(backup.size() == container_.size());

    auto out_iter = container_.begin();

    for (auto const& element : backup)
    {
        *(out_iter++)->corner = element;
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
