#pragma once

#include <boost/noncopyable.hpp>
#include <kloss/world.hpp>

namespace kloss {
namespace creator {

struct document : boost::noncopyable
{
    world world;
};

} // namespace creator
} // namespace kloss
