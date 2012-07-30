#include "vertex_selection.hpp"
#include <kloss/algorithm.hpp>

namespace kloss {
namespace creator {

bool const vertex_selection::contains(vertex_ref const& element) const
{
    return kloss::contains(container_, element);
}

void vertex_selection::insert(vertex_ref const& element)
{
    container_.push_back(element);
}

void vertex_selection::remove(vertex_ref const& element)
{
    kloss::remove(container_, element);
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
        backup.push_back(element.corner());
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

std::vector<vertex_ref>::iterator vertex_selection::begin()
{
    return container_.begin();
}

std::vector<vertex_ref>::iterator vertex_selection::end()
{
    return container_.end();
}

} // namespace creator
} // namespace kloss
