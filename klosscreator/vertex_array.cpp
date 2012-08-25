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
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), &front.position.X);
        glDrawArrays(GL_LINES, 0, array.size());
        glPopClientAttrib();
    }
}

void draw(vertex_array const& array, Vec3 const& position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.X, position.Y, position.Z);
    draw(array);
    glPopMatrix();
}

void append_line(vertex_array& array, color3ub const& color, Vec3 const& begin, Vec3 const& end)
{
    array.push_back({color, begin});
    array.push_back({color, end});
}

vertex_array make_grid(int size)
{
    vertex_array array;
    array.reserve(2 * (4 * size + 4));

    for (float i = -size; i <= size; ++i)
    {
        float min = -size;
        float max = i != 0 ? size : 0.0f;

        append_line(array, {255, 255, 255}, {i, min, 0.0f}, {i, max, 0.0f});
        append_line(array, {255, 255, 255}, {min, i, 0.0f}, {max, i, 0.0f});
    }

    {
        append_line(array, {255, 0, 0}, {0.0f, 0.0f, 0.0f}, {float(size), 0.0f, 0.0f});
        append_line(array, {0, 255, 0}, {0.0f, 0.0f, 0.0f}, {0.0f, float(size), 0.0f});
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
