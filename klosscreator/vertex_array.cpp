#include "vertex_array.hpp"
#include <GL/gl.h>
#include <kloss/algorithm.hpp>

void draw(vertex_array const& array)
{
    vertex const* vertices = array.data();

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(struct vertex), &vertices->color);
    glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &vertices->position);
    glDrawArrays(GL_LINES, 0, array.size());
    glPopClientAttrib();
}

void draw_at(vertex_array const& array, vec3 const& position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    draw(array);
    glPopMatrix();
}

vertex_array create_grid(int size)
{
    vertex_array array;

    for (float i = -size; i <= size; ++i)
    {
        float min = -size;
        float max = i != 0 ? size : 0.0f;

        append(array,
        {
            {{255, 255, 255}, {i, min, 0.0f}},
            {{255, 255, 255}, {i, max, 0.0f}},
            {{255, 255, 255}, {min, i, 0.0f}},
            {{255, 255, 255}, {max, i, 0.0f}},
        });
   }

    {
        append(array,
        {
            {{255, 0, 0}, {0.0f,        0.0f,        0.0f}},
            {{255, 0, 0}, {float(size), 0.0f,        0.0f}},
            {{0, 255, 0}, {0.0f,        0.0f,        0.0f}},
            {{0, 255, 0}, {0.0f,        float(size), 0.0f}},
        });
    }

    return array;
}

vertex_array create_cursor(float size)
{
    vertex_array array;

    append(array,
    {
        {{255, 0, 0}, {-size,  0.0f,  0.0f}},
        {{255, 0, 0}, { size,  0.0f,  0.0f}},
        {{0, 255, 0}, { 0.0f, -size,  0.0f}},
        {{0, 255, 0}, { 0.0f,  size,  0.0f}},
        {{0, 0, 255}, { 0.0f,  0.0f, -size}},
        {{0, 0, 255}, { 0.0f,  0.0f,  size}},
    });

    return array;
}
