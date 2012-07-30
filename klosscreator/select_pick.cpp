#include "select_pick.hpp"

namespace kloss {
namespace creator {

bool const is_initialized(pick const& pick)
{
    return pick.block != nullptr;
}

block_ptr const& get_object(pick const& pick)
{
    return pick.block;
}

cml::vector3f const& get_intersection(pick const& pick)
{
    return pick.intersection;
}

bool const is_initialized(boost::optional<vertex_ref> const& pick)
{
    return pick;
}

vertex_ref const& get_object(boost::optional<vertex_ref> const& pick)
{
    return *pick;
}

cml::vector3f const get_intersection(boost::optional<vertex_ref> const& pick)
{
    return pick->to_vector();
}

} // namespace creator
} // namespace kloss
