#include "boundingbox.h"
#include <kloss/algorithm.h>
#include <float.h>

void init_boundingbox(boundingbox *bbox)
{
    bbox->lower.x = FLT_MAX;
    bbox->lower.y = FLT_MAX;
    bbox->lower.z = FLT_MAX;
    bbox->upper.x = FLT_MIN;
    bbox->upper.y = FLT_MIN;
    bbox->upper.z = FLT_MIN;
}

void expand_boundingbox(boundingbox *lhs, boundingbox const *other)
{
    lhs->lower.x = minf(lhs->lower.x, other->lower.x);
    lhs->lower.y = minf(lhs->lower.y, other->lower.y);
    lhs->lower.z = minf(lhs->lower.z, other->lower.z);
    lhs->upper.x = maxf(lhs->upper.x, other->upper.x);
    lhs->upper.y = maxf(lhs->upper.y, other->upper.y);
    lhs->upper.z = maxf(lhs->upper.z, other->upper.z);
}

void translate_boundingbox(boundingbox *bbox, vec3 const *units)
{
    vec3_add(&bbox->lower, &bbox->lower, units);
    vec3_add(&bbox->upper, &bbox->upper, units);
}
