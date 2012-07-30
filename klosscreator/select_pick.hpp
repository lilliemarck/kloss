#pragma once

#include <kloss/block.hpp>
#include <kloss/world.hpp>

namespace kloss {
namespace creator {

bool const is_initialized(pick const& pick);
block_ptr const& get_object(pick const& pick);
cml::vector3f const& get_intersection(pick const& pick);

bool const is_initialized(boost::optional<vertex_ref> const& pick);
vertex_ref const& get_object(boost::optional<vertex_ref> const& pick);
cml::vector3f const get_intersection(boost::optional<vertex_ref> const& pick);

template <typename Selection, typename Pick>
bool const single_select(Selection& selection, Pick const& pick)
{
    if (is_initialized(pick))
    {
        auto const& element = get_object(pick);

        if (!selection.contains(element))
        {
            selection.clear();
            selection.insert(element);
        }

        return true;
    }
    else
    {
        selection.clear();
    }

    return false;
}

template <typename Selection, typename Pick>
bool const multi_select(Selection& selection, Pick const& pick)
{
    if (is_initialized(pick))
    {
        auto const& element = get_object(pick);

        if (!selection.contains(element))
        {
            selection.insert(element);
            return true;
        }
        else
        {
            selection.remove(element);
        }
    }

    return false;
}

} // namespace creator
} // namespace kloss
