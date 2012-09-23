#include "block.h"
#include <kloss/buffer.h>
#include <stdlib.h>
#include <string.h>

vec3 cornertop(corner const *corner)
{
    return (vec3){corner->x, corner->y, corner->top};
}

vec3 cornerbottom(corner const *corner)
{
    return (vec3){corner->x, corner->y, corner->bottom};
}

void translate_corner(corner *corner, vec3 const *units)
{
    corner->x      += units->x;
    corner->y      += units->y;
    corner->top    += units->z;
    corner->bottom += units->z;
}

block *create_block(void)
{
    return calloc(1, sizeof(block));
}

block *copy_block(block const *block)
{
    struct block *copy = malloc(sizeof(block));
    memcpy(copy, block, sizeof(block));
    return copy;
}

void destroy_block(block *block)
{
    free(block);
}

void translate_block(block *block, vec3 const *units)
{
    translate_corner(&block->corners[0], units);
    translate_corner(&block->corners[1], units);
    translate_corner(&block->corners[2], units);
    translate_corner(&block->corners[3], units);
}

void get_block_triangles(block const *block, buffer *buffer)
{
    triangle triangles[12] =
    {
        // front
        {cornertop(&block->corners[0]), cornerbottom(&block->corners[0]),    cornertop(&block->corners[1])},
        {cornertop(&block->corners[1]), cornerbottom(&block->corners[0]), cornerbottom(&block->corners[1])},

        // right
        {cornertop(&block->corners[1]), cornerbottom(&block->corners[1]),    cornertop(&block->corners[2])},
        {cornertop(&block->corners[2]), cornerbottom(&block->corners[1]), cornerbottom(&block->corners[2])},

        // back
        {cornertop(&block->corners[2]), cornerbottom(&block->corners[2]),    cornertop(&block->corners[3])},
        {cornertop(&block->corners[3]), cornerbottom(&block->corners[2]), cornerbottom(&block->corners[3])},

        // left
        {cornertop(&block->corners[3]), cornerbottom(&block->corners[3]),    cornertop(&block->corners[0])},
        {cornertop(&block->corners[0]), cornerbottom(&block->corners[3]), cornerbottom(&block->corners[0])},

        // CornerTop
        {cornertop(&block->corners[3]), cornertop(&block->corners[0]), cornertop(&block->corners[2])},
        {cornertop(&block->corners[2]), cornertop(&block->corners[0]), cornertop(&block->corners[1])},

        // bottom
        {cornerbottom(&block->corners[3]), cornerbottom(&block->corners[2]), cornerbottom(&block->corners[0])},
        {cornerbottom(&block->corners[0]), cornerbottom(&block->corners[2]), cornerbottom(&block->corners[1])}
    };

    append_buffer(buffer, &triangles, sizeof(triangles));
}
