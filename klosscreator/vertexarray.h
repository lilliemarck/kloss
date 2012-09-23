#pragma once

#include <kloss/math.h>
#include <stdint.h>

struct buffer;

typedef struct color3ub color3ub;
typedef struct vertex vertex;

struct color3ub
{
    uint8_t red, green, blue;
};

struct vertex
{
    color3ub color;
    vec3 position;
};

void draw(struct buffer *array);
void draw_at(struct buffer *array, vec3 position);

struct buffer *create_grid(int size);
struct buffer *create_cursor(float size);
