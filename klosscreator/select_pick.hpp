#pragma once

#include <kloss/block.hpp>
#include <kloss/group.hpp>

namespace kloss {
namespace creator {

bool const is_initialized(pick const& pick);
block_ptr& get_object(pick& pick);
cml::vector3f const& get_intersection(pick const& pick);

bool const is_initialized(boost::optional<corner_ref> const& pick);
corner_ref& get_object(boost::optional<corner_ref>& pick);
cml::vector3f const get_intersection(boost::optional<corner_ref> const& pick);

template <typename Selection, typename Pick>
bool const single_select(Selection& selection, Pick& pick)
{
    if (is_initialized(pick))
    {
        auto& element = get_object(pick);

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
bool const multi_select(Selection& selection, Pick& pick)
{
    if (is_initialized(pick))
    {
        auto& element = get_object(pick);

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
