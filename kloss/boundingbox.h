#pragma once

#include <kloss/math.h>

typedef struct boundingbox boundingbox;

struct boundingbox
{
    vec3 lower;
    vec3 upper;
};

void init_boundingbox(boundingbox *bbox);
void expand_boundingbox(boundingbox *bbox, boundingbox const *other);
void translate_boundingbox(boundingbox *bbox, vec3 const *units);
