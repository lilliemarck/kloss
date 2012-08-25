#include "select_pick.hpp"

namespace kloss {
namespace creator {

bool const is_initialized(pick const& pick)
{
    return pick.block != nullptr;
}

block_ptr& get_object(pick& pick)
{
    return pick.block;
}

Vec3 const& get_intersection(pick const& pick)
{
    return pick.intersection;
}

bool const is_initialized(boost::optional<corner_ref> const& pick)
{
    return pick;
}

corner_ref& get_object(boost::optional<corner_ref>& pick)
{
    return *pick;
}

Vec3 get_intersection(boost::optional<corner_ref> const& pick)
{
    return to_vector(*pick);
}

} // namespace creator
} // namespace kloss
