#include "boundingbox.h"
#include <kloss/algorithm.h>
#include <float.h>

void InitBoundingBox(BoundingBox *bbox)
{
    bbox->Lower.X = FLT_MAX;
    bbox->Lower.Y = FLT_MAX;
    bbox->Lower.Z = FLT_MAX;
    bbox->Upper.X = FLT_MIN;
    bbox->Upper.Y = FLT_MIN;
    bbox->Upper.Z = FLT_MIN;
}

void ExpandBoundingBox(BoundingBox *lhs, BoundingBox const *other)
{
    lhs->Lower.X = Minf(lhs->Lower.X, other->Lower.X);
    lhs->Lower.Y = Minf(lhs->Lower.Y, other->Lower.Y);
    lhs->Lower.Z = Minf(lhs->Lower.Z, other->Lower.Z);
    lhs->Upper.X = Maxf(lhs->Upper.X, other->Upper.X);
    lhs->Upper.Y = Maxf(lhs->Upper.Y, other->Upper.Y);
    lhs->Upper.Z = Maxf(lhs->Upper.Z, other->Upper.Z);
}

void TranslateBoundingBox(BoundingBox *bbox, Vec3 const *units)
{
    Vec3Add(&bbox->Lower, &bbox->Lower, units);
    Vec3Add(&bbox->Upper, &bbox->Upper, units);
}
