#include "block.h"
#include <kloss/buffer.h>
#include <stdlib.h>
#include <string.h>

Vec3 CornerTop(Corner const *corner)
{
    return (Vec3){corner->X, corner->Y, corner->Top};
}

Vec3 CornerBottom(Corner const *corner)
{
    return (Vec3){corner->X, corner->Y, corner->Bottom};
}

void TranslateCorner(Corner *corner, Vec3 const *units)
{
    corner->X      += units->X;
    corner->Y      += units->Y;
    corner->Top    += units->Z;
    corner->Bottom += units->Z;
}

Block *CreateBlock(void)
{
    return calloc(1, sizeof(Block));
}

Block *CopyBlock(Block const *block)
{
    Block *copy = malloc(sizeof(Block));
    memcpy(copy, block, sizeof(Block));
    return copy;
}

void DestroyBlock(Block *block)
{
    free(block);
}

void TranslateBlock(Block *block, Vec3 const *units)
{
    TranslateCorner(&block->Corners[0], units);
    TranslateCorner(&block->Corners[1], units);
    TranslateCorner(&block->Corners[2], units);
    TranslateCorner(&block->Corners[3], units);
}

void GetBlockTriangles(Block const *block, Buffer *buffer)
{
    Triangle triangles[12] =
    {
        // front
        {CornerTop(&block->Corners[0]), CornerBottom(&block->Corners[0]),    CornerTop(&block->Corners[1])},
        {CornerTop(&block->Corners[1]), CornerBottom(&block->Corners[0]), CornerBottom(&block->Corners[1])},

        // right
        {CornerTop(&block->Corners[1]), CornerBottom(&block->Corners[1]),    CornerTop(&block->Corners[2])},
        {CornerTop(&block->Corners[2]), CornerBottom(&block->Corners[1]), CornerBottom(&block->Corners[2])},

        // back
        {CornerTop(&block->Corners[2]), CornerBottom(&block->Corners[2]),    CornerTop(&block->Corners[3])},
        {CornerTop(&block->Corners[3]), CornerBottom(&block->Corners[2]), CornerBottom(&block->Corners[3])},

        // left
        {CornerTop(&block->Corners[3]), CornerBottom(&block->Corners[3]),    CornerTop(&block->Corners[0])},
        {CornerTop(&block->Corners[0]), CornerBottom(&block->Corners[3]), CornerBottom(&block->Corners[0])},

        // CornerTop
        {CornerTop(&block->Corners[3]), CornerTop(&block->Corners[0]), CornerTop(&block->Corners[2])},
        {CornerTop(&block->Corners[2]), CornerTop(&block->Corners[0]), CornerTop(&block->Corners[1])},

        // bottom
        {CornerBottom(&block->Corners[3]), CornerBottom(&block->Corners[2]), CornerBottom(&block->Corners[0])},
        {CornerBottom(&block->Corners[0]), CornerBottom(&block->Corners[2]), CornerBottom(&block->Corners[1])}
    };

    AppendToBuffer(buffer, &triangles, sizeof(triangles));
}
