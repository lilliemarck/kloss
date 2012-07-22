#include "cursor.hpp"
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

cursor::cursor(float size)
{
    append_line({255, 0, 0}, {-size, 0.0f, 0.0f}, {size, 0.0f, 0.0f});
    append_line({0, 255, 0}, {0.0f, -size, 0.0f}, {0.0f, size, 0.0f});
    append_line({0, 0, 255}, {0.0f, 0.0f, -size}, {0.0f, 0.0f, size});
}

void cursor::draw() const
{
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

    set_vertex_array_pointer(sizeof(vertex), vertices_.data()->color);
    set_vertex_array_pointer(sizeof(vertex), vertices_.data()->position);

    glDrawArrays(GL_LINES, 0, vertices_.size());
    glPopClientAttrib();
}

void cursor::append_line(color3ub const& color, cml::vector3f const& begin, cml::vector3f const& end)
{
    vertices_.push_back({color, begin});
    vertices_.push_back({color, end});
}

void draw(cursor const& cursor, cml::vector3f const& position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(position[0], position[1], position[2]);
    cursor.draw();

    glPopMatrix();
}

} // namespace creator
} // namespace kloss
