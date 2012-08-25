#include "cornerselection.h"
#include "kloss/block.h"
#include "kloss/buffer.h"
#include "kloss/tarray.h"
#include "kloss/cornerref.h"
#include "klosscreator/pick.h"
#include <assert.h>
#include <stdlib.h>

struct CornerSelection
{
    DECLARE_TARRAY(CornerRef, CornerRefs);
};

CornerSelection *CreateCornerSelection(void)
{
    return calloc(1, sizeof(CornerSelection));
}

void DestroyCornerSelection(CornerSelection *selection)
{
    if (selection)
    {
        FREE_TARRAY(selection->CornerRefs);
        free(selection);
    }
}

size_t SelectedCornerCount(CornerSelection *selection)
{
    return TARRAY_LENGTH(selection->CornerRefs);
}

CornerRef *SelectedCorners(CornerSelection *selection)
{
    return selection->CornerRefs.Begin;
}

void BackupCornerSelection(CornerSelection *selection, struct Buffer *buffer)
{
    for (CornerRef *i = selection->CornerRefs.Begin; i != selection->CornerRefs.End; ++i)
    {
        AppendToBuffer(buffer, i->Corner, sizeof(Corner));
    }
}

void RestoreCornerSelection(CornerSelection *selection, struct Buffer *buffer)
{
    assert(BufferSize(buffer) / sizeof(Corner) == TARRAY_LENGTH(selection->CornerRefs));

    Corner *backup = BufferData(buffer);
    size_t cornerCount = TARRAY_LENGTH(selection->CornerRefs);

    for (size_t i = 0; i < cornerCount; ++i)
    {
        *selection->CornerRefs.Begin[i].Corner = backup[i];
    }
}

static bool IsSelected(void *data, void *element)
{
    CornerSelection *selection = data;
    CornerRef *cornerRef = element;

    for (CornerRef *i = selection->CornerRefs.Begin; i != selection->CornerRefs.End; ++i)
    {
        if (i->Corner == cornerRef->Corner && i->Flags & cornerRef->Flags)
        {
            return true;
        }
    }

    return false;
}

static void Select(void *data, void *element)
{
    CornerSelection *selection = data;
    CornerRef *cornerRef = element;

    for (CornerRef *i = selection->CornerRefs.Begin; i != selection->CornerRefs.End; ++i)
    {
        if (i->Corner == cornerRef->Corner)
        {
            i->Flags |= cornerRef->Flags;
            return;
        }
    }

    PUSH_TARRAY(selection->CornerRefs, *cornerRef);
}

static void Deselect(void *data, void *element)
{
    CornerSelection *selection = data;
    CornerRef *cornerRef = element;

    for (CornerRef *i = selection->CornerRefs.Begin; i != selection->CornerRefs.End; ++i)
    {
        if (i->Corner == cornerRef->Corner)
        {
            i->Flags &= ~cornerRef->Flags;

            if (i->Flags == 0)
            {
                ERASE_TARRAY_ITERATOR(selection->CornerRefs, i);
            }

            return;
        }
    }
}

static void DeselectAll(void *data)
{
    CornerSelection *selection = data;
    CLEAR_TARRAY(selection->CornerRefs);
}

static PickInterface interface =
{
    IsSelected,
    Select,
    Deselect,
    DeselectAll
};

bool SinglePickCorner(CornerSelection *selection, CornerRef *pick)
{
    return SinglePick(&interface, selection, pick);
}

bool MultiPickCorner(CornerSelection *selection, CornerRef *pick)
{
    return MultiPick(&interface, selection, pick);
}
