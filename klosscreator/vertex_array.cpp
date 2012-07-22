#include "vertex_array.hpp"
#include <GL/gl.h>

namespace kloss {
namespace creator {

void draw(vertex_array const& array)
{
    if (!array.empty())
    {
        vertex_array::value_type const& front = array.front();

        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertex), front.color.data());
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), front.position.data());
        glDrawArrays(GL_LINES, 0, array.size());
        glPopClientAttrib();
    }
}

void draw(vertex_array const& array, cml::vector3f const& position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);
    draw(array);
    glPopMatrix();
}

void append_line(vertex_array& array, color3ub const& color, cml::vector3f const& begin, cml::vector3f const& end)
{
    array.push_back({color, begin});
    array.push_back({color, end});
}

vertex_array make_grid(int size)
{
    vertex_array array;
    array.reserve(2 * (4 * size + 5));

    for (float i = -size; i <= size; ++i)
    {
        float min = -size;
        float max = i != 0 ? size : 0.0f;

        append_line(array, {255, 255, 255}, {i, 0.0f, min}, {i, 0.0f, max});
        append_line(array, {255, 255, 255}, {min, 0.0f, i}, {max, 0.0f, i});
    }

    {
        append_line(array, {255, 0, 0}, {0.0f, 0.0f, 0.0f}, {size, 0.0f, 0.0f});
        append_line(array, {0, 255, 0}, {0.0f, 0.0f, 0.0f}, {0.0f, size, 0.0f});
        append_line(array, {0, 0, 255}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, size});
    }

    return array;
}

vertex_array make_cursor(float size)
{
    vertex_array array;
    array.reserve(6);

    append_line(array, {255, 0, 0}, {-size, 0.0f, 0.0f}, {size, 0.0f, 0.0f});
    append_line(array, {0, 255, 0}, {0.0f, -size, 0.0f}, {0.0f, size, 0.0f});
    append_line(array, {0, 0, 255}, {0.0f, 0.0f, -size}, {0.0f, 0.0f, size});

    return array;
}

} // namespace creator
} // namespace kloss
