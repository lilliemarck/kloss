#include "cornerref.h"
#include <kloss/block.h>
#include <stddef.h>

static void init_cornerref(cornerref *ref, block *block, size_t index)
{
   ref->corner = block->corners + index;
   ref->flags = CORNERREF_TOP | CORNERREF_BOTTOM;
}

void make_cornerrefs(cornerref refs[4], block *block)
{
    init_cornerref(refs + 0, block, 0);
    init_cornerref(refs + 1, block, 1);
    init_cornerref(refs + 2, block, 2);
    init_cornerref(refs + 3, block, 3);
}

void translate_cornerref(cornerref *ref, vec3 const *units)
{
    ref->corner->x += units->x;
    ref->corner->y += units->y;

    if (ref->flags & CORNERREF_TOP)
    {
        ref->corner->top += units->z;
    }

    if (ref->flags & CORNERREF_BOTTOM)
    {
        ref->corner->bottom += units->z;
    }
}

vec3 cornerref_position(cornerref const *ref)
{
    bool is_top = ref->flags & CORNERREF_TOP;
    return is_top ? cornertop(ref->corner) : cornerbottom(ref->corner);
}
