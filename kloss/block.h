#pragma once

#include <kloss/geometry.h>
#include <kloss/math.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Buffer;

typedef struct Block Block;
typedef struct Corner Corner;

struct Corner
{
    float X;
    float Y;
    float Top;
    float Bottom;
};

struct Block
{
    Corner Corners[4];
};

Vec3 CornerTop(Corner const *corner);
Vec3 CornerBottom(Corner const *corner);
void TranslateCorner(Corner *corner, Vec3 const *units);

Block *CreateBlock(void);
Block *CopyBlock(Block const *block);
void DestroyBlock(Block *block);
void TranslateBlock(Block *block, Vec3 const *units);
void GetBlockTriangles(Block const *block, struct Buffer *buffer);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
