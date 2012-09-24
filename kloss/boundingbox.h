#pragma once

#include <kloss/math.h>

typedef struct BoundingBox BoundingBox;

struct BoundingBox
{
    Vec3 Lower;
    Vec3 Upper;
};

void InitBoundingBox(BoundingBox *bbox);
void ExpandBoundingBox(BoundingBox *bbox, BoundingBox const *other);
void TranslateBoundingBox(BoundingBox *bbox, Vec3 const *units);
