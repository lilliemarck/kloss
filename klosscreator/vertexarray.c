#include "vertexarray.h"
#include <kloss/buffer.h>
#include <GL/gl.h>

void draw(struct Buffer *array)
{
    vertex *data = BufferData(array);
    size_t count = BufferSize(array) / sizeof(vertex);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertex), &data->color);
    glVertexPointer(3, GL_FLOAT, sizeof(vertex), &data->position);
    glDrawArrays(GL_LINES, 0, count);
    glPopClientAttrib();
}

void draw_at(struct Buffer *array, Vec3 position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.X, position.Y, position.Z);
    draw(array);
    glPopMatrix();
}

Buffer *create_grid(int size)
{
    Buffer *array = CreateBuffer();

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

        AppendToBuffer(array, lines, sizeof(lines));
   }

    {
        vertex lines[] =
        {
            {{255, 0, 0}, {0.0f, 0.0f, 0.0f}},
            {{255, 0, 0}, {size, 0.0f, 0.0f}},
            {{0, 255, 0}, {0.0f, 0.0f, 0.0f}},
            {{0, 255, 0}, {0.0f, size, 0.0f}},
        };

        AppendToBuffer(array, lines, sizeof(lines));
    }

    return array;
}

Buffer *create_cursor(float size)
{
    Buffer *array = CreateBuffer();

    vertex lines[] =
    {
        {{255, 0, 0}, {-size,  0.0f,  0.0f}},
        {{255, 0, 0}, { size,  0.0f,  0.0f}},
        {{0, 255, 0}, { 0.0f, -size,  0.0f}},
        {{0, 255, 0}, { 0.0f,  size,  0.0f}},
        {{0, 0, 255}, { 0.0f,  0.0f, -size}},
        {{0, 0, 255}, { 0.0f,  0.0f,  size}},
    };

    AppendToBuffer(array, lines, sizeof(lines));
    return array;
}
