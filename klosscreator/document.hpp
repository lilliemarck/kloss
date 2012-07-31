#pragma once

#include <boost/noncopyable.hpp>
#include <kloss/world.hpp>
#include <klosscreator/block_selection.hpp>

namespace kloss {
namespace creator {

struct document : boost::noncopyable
{
    world world;
    block_selection block_selection;
};

} // namespace creator
} // namespace kloss
