#include "grid.hpp"
#include <GL/gl.h>

namespace kloss {
namespace creator {
namespace {

void set_vertex_array_pointer(size_t stride, color3ub const& element)
{
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, stride, element.data());
};

void set_vertex_array_pointer(size_t stride, cml::vector3f const& element)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, element.data());
};

} // namespace

grid::grid(int size)
{
    if (size == 0)
    {
        return;
    }

    for (float i = -size; i <= size; ++i)
    {
        float min = -size;
        float max = i != 0 ? size : 0.0f;

        append_line({255, 255, 255}, {i, 0.0f, min}, {i, 0.0f, max});
        append_line({255, 255, 255}, {min, 0.0f, i}, {max, 0.0f, i});
    }

    {
        float max = size;

        append_line({255, 0, 0}, {0.0f, 0.0f, 0.0f}, {max, 0.0f, 0.0f});
        append_line({0, 255, 0}, {0.0f, 0.0f, 0.0f}, {0.0f, max, 0.0f});
        append_line({0, 0, 255}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, max});
    }
}

void grid::draw() const
{
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

    set_vertex_array_pointer(sizeof(vertex), vertices_.data()->color);
    set_vertex_array_pointer(sizeof(vertex), vertices_.data()->position);

    glDrawArrays(GL_LINES, 0, vertices_.size());
    glPopClientAttrib();
}

void grid::append_line(color3ub const& color, cml::vector3f const& begin, cml::vector3f const& end)
{
    vertices_.push_back({color, begin});
    vertices_.push_back({color, end});
}

} // namespace creator
} // namespace kloss
