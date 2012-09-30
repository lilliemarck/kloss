#include "cornerselection.h"
#include "kloss/block.h"
#include "kloss/buffer.h"
#include "kloss/tarray.h"
#include "kloss/cornerref.h"
#include "klosscreator/pick.h"
#include <assert.h>
#include <stdlib.h>

struct cornerselection
{
    DECLARE_TARRAY(cornerref, cornerrefs);
};

cornerselection *create_cornerselection(void)
{
    return calloc(1, sizeof(struct cornerselection));
}

void destroy_cornerselection(cornerselection *selection)
{
    if (selection)
    {
        FREE_TARRAY(selection->cornerrefs);
        free(selection);
    }
}

size_t selected_corner_count(cornerselection *selection)
{
    return TARRAY_LENGTH(selection->cornerrefs);
}

cornerref *selected_corners(cornerselection *selection)
{
    return selection->cornerrefs.begin;
}

void backup_cornerselection(cornerselection *selection, struct buffer *buffer)
{
    for (cornerref *i = selection->cornerrefs.begin; i != selection->cornerrefs.end; ++i)
    {
        append_buffer(buffer, i->corner, sizeof(struct corner));
    }
}

void restore_cornerselection(cornerselection *selection, struct buffer *buffer)
{
    assert(buffer_count(buffer, sizeof(struct corner)) == TARRAY_LENGTH(selection->cornerrefs));

    corner *backup = buffer_data(buffer);
    size_t cornercount = TARRAY_LENGTH(selection->cornerrefs);

    for (size_t i = 0; i < cornercount; ++i)
    {
        *selection->cornerrefs.begin[i].corner = backup[i];
    }
}

static bool is_selected(void *data, void *element)
{
    cornerselection *selection = data;
    cornerref *ref = element;

    for (cornerref *i = selection->cornerrefs.begin; i != selection->cornerrefs.end; ++i)
    {
        if (i->corner == ref->corner && i->flags & ref->flags)
        {
            return true;
        }
    }

    return false;
}

static void select(void *data, void *element)
{
    cornerselection *selection = data;
    cornerref *ref = element;

    for (cornerref *i = selection->cornerrefs.begin; i != selection->cornerrefs.end; ++i)
    {
        if (i->corner == ref->corner)
        {
            i->flags |= ref->flags;
            return;
        }
    }

    PUSH_TARRAY(selection->cornerrefs, *ref);
}

static void deselect(void *data, void *element)
{
    cornerselection *selection = data;
    cornerref *ref = element;

    for (cornerref *i = selection->cornerrefs.begin; i != selection->cornerrefs.end; ++i)
    {
        if (i->corner == ref->corner)
        {
            i->flags &= ~ref->flags;

            if (i->flags == 0)
            {
                ERASE_TARRAY_ITERATOR(selection->cornerrefs, i);
            }

            return;
        }
    }
}

static void deselect_all(void *data)
{
    cornerselection *selection = data;
    CLEAR_TARRAY(selection->cornerrefs);
}

static pickprocs interface =
{
    is_selected,
    select,
    deselect,
    deselect_all
};

bool single_pick_corner(cornerselection *selection, cornerref *pick)
{
    return single_pick(&interface, selection, pick);
}

bool multi_pick_corner(cornerselection *selection, cornerref *pick)
{
    return multi_pick(&interface, selection, pick);
}
