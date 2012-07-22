#pragma once

#include <cml/cml.h>
#include <kloss/color.hpp>

namespace kloss {
namespace creator {

class cursor
{
public:
    cursor(float size);
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

void draw(cursor const& cursor, cml::vector3f const& position);

} // namespace creator
} // namespace kloss
