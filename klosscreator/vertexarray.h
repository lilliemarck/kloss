#pragma once

#include <kloss/math.h>
#include <stdint.h>

struct Buffer;

typedef struct color3ub color3ub;
typedef struct vertex vertex;

struct color3ub
{
    uint8_t red, green, blue;
};

struct vertex
{
    color3ub color;
    Vec3 position;
};

void draw(struct Buffer *array);
void draw_at(struct Buffer *array, Vec3 position);

struct Buffer *create_grid(int size);
struct Buffer *create_cursor(float size);
