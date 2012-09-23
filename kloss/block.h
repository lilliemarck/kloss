#pragma once

#include <kloss/geometry.h>
#include <kloss/math.h>

struct buffer;

typedef struct block block;
typedef struct corner corner;

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

struct block
{
    corner corners[4];
};

vec3 cornertop(corner const *corner);
vec3 cornerbottom(corner const *corner);
void translate_corner(corner *corner, vec3 const *units);

block *create_block(void);
block *copy_block(block const *block);
void destroy_block(block *block);
void translate_block(block *block, vec3 const *units);
void get_block_triangles(block const *block, struct buffer *buffer);
