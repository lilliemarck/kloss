#pragma once

#include <cml/cml.h>
#include <kloss/color.hpp>

namespace kloss {
namespace creator {

class grid
{
public:
    grid(int size);
    void draw() const;

private:
    struct vertex
    {
        color3ub color;
        cml::vector3f position;
    };

    void append_line(color3ub const& color, cml::vector3f const& begin, cml::vector3f const& end);

    std::vector<vertex> vertices_;
};

} // namespace creator
} // namespace kloss
