#include "cornerref.h"
#include <kloss/block.h>
#include <stddef.h>

static void InitCornerRef(CornerRef *ref, Block *block, size_t index)
{
   ref->Corner = block->Corners + index;
   ref->Flags = CORNER_REF_TOP | CORNER_REF_BOTTOM;
}

void MakeCornerRefs(CornerRef refs[4], Block *block)
{
    InitCornerRef(refs + 0, block, 0);
    InitCornerRef(refs + 1, block, 1);
    InitCornerRef(refs + 2, block, 2);
    InitCornerRef(refs + 3, block, 3);
}

void TranslateCornerRef(CornerRef *ref, Vec3 const *units)
{
    ref->Corner->X += units->X;
    ref->Corner->Y += units->Y;

    if (ref->Flags & CORNER_REF_TOP)
    {
        ref->Corner->Top += units->Z;
    }

    if (ref->Flags & CORNER_REF_BOTTOM)
    {
        ref->Corner->Bottom += units->Z;
    }
}

Vec3 CornerRefPosition(CornerRef const *ref)
{
    bool is_top = ref->Flags & CORNER_REF_TOP;
    return is_top ? CornerTop(ref->Corner) : CornerBottom(ref->Corner);
}
