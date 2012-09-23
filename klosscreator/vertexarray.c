#include "vertexarray.h"
#include <kloss/buffer.h>
#include <GL/gl.h>

void draw(struct buffer *array)
{
    vertex *data = buffer_data(array);
    size_t count = buffer_size(array) / sizeof(vertex);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertex), &data->color);
    glVertexPointer(3, GL_FLOAT, sizeof(vertex), &data->position);
    glDrawArrays(GL_LINES, 0, count);
    glPopClientAttrib();
}

void draw_at(struct buffer *array, vec3 position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    draw(array);
    glPopMatrix();
}

buffer *create_grid(int size)
{
    buffer *array = create_buffer();

    for (float i = -size; i <= size; ++i)
    {
        float min = -size;
        float max = i != 0 ? size : 0.0f;

        vertex lines[] =
        {
            {{255, 255, 255}, {i, min, 0.0f}},
            {{255, 255, 255}, {i, max, 0.0f}},
            {{255, 255, 255}, {min, i, 0.0f}},
            {{255, 255, 255}, {max, i, 0.0f}},
        };

        append_buffer(array, lines, sizeof(lines));
   }

    {
        vertex lines[] =
        {
            {{255, 0, 0}, {0.0f, 0.0f, 0.0f}},
            {{255, 0, 0}, {size, 0.0f, 0.0f}},
            {{0, 255, 0}, {0.0f, 0.0f, 0.0f}},
            {{0, 255, 0}, {0.0f, size, 0.0f}},
        };

        append_buffer(array, lines, sizeof(lines));
    }

    return array;
}

buffer *create_cursor(float size)
{
    buffer *array = create_buffer();

    vertex lines[] =
    {
        {{255, 0, 0}, {-size,  0.0f,  0.0f}},
        {{255, 0, 0}, { size,  0.0f,  0.0f}},
        {{0, 255, 0}, { 0.0f, -size,  0.0f}},
        {{0, 255, 0}, { 0.0f,  size,  0.0f}},
        {{0, 0, 255}, { 0.0f,  0.0f, -size}},
        {{0, 0, 255}, { 0.0f,  0.0f,  size}},
    };

    append_buffer(array, lines, sizeof(lines));
    return array;
}
