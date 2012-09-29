#include "blockselection.h"
#include <kloss/algorithm.h>
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
#include <kloss/tarray.h>
#include <klosscreator/pick.h>
#include <assert.h>
#include <stdlib.h>

struct blockselection
{
    struct group *rootgroup;
    DECLARE_TARRAY(struct block*, blocks);
    DECLARE_TARRAY(struct group*, groups);
};

blockselection *create_blockselection(struct group *rootgroup)
{
    struct blockselection *selection = calloc(1, sizeof(struct blockselection));
    selection->rootgroup = rootgroup;
    return selection;
}

void destroy_blockselection(blockselection *selection)
{
    if (selection)
    {
        FREE_TARRAY(selection->groups);
        FREE_TARRAY(selection->blocks);
        free(selection);
    }
}

/**
 * Returns true if the block is selected directly or indirectly through a child
 * group.
 */
static bool is_block_selected(struct blockselection *selection, struct blockref ref)
{
    if (find_ptr(selection->blocks.begin, selection->blocks.end, ref.block))
    {
        return true;
    }

    struct group *child = get_child_by_descendant(selection->rootgroup, ref.group);
    return find_ptr(selection->groups.begin, selection->groups.end, child);
}

void select_block(struct blockselection *selection, struct blockref ref)
{
    if (is_block_selected(selection, ref))
    {
        return;
    }

    if (ref.group == selection->rootgroup)
    {
        PUSH_TARRAY(selection->blocks, ref.block);
    }

    struct group *child = get_child_by_descendant(selection->rootgroup, ref.group);

    if (child)
    {
        PUSH_TARRAY(selection->groups, child);
    }
}

void deselect_block(blockselection *selection, struct blockref ref)
{
    for (struct block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        if (*i == ref.block)
        {
            ERASE_TARRAY_ITERATOR(selection->blocks, i);
            return;
        }
    }

    struct group *child = get_child_by_descendant(selection->rootgroup, ref.group);

    for (struct group **i = selection->groups.begin; i != selection->groups.end; ++i)
    {
        if (*i == child)
        {
            ERASE_TARRAY_ITERATOR(selection->groups, i);
            return;
        }
    }
}

void deselect_all_blocks(blockselection *selection)
{
    CLEAR_TARRAY(selection->blocks);
    CLEAR_TARRAY(selection->groups);
}

size_t selected_block_count(blockselection *selection)
{
    return TARRAY_LENGTH(selection->blocks);
}

block **selected_blocks(blockselection *selection)
{
    return selection->blocks.begin;
}

void backup_blockselection(blockselection *selection, buffer *buffer)
{
    for (block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        append_buffer(buffer, *i, sizeof(struct block));
    }
}

void restore_blockselection(blockselection *selection, buffer *buffer)
{
    assert(buffer_size(buffer) / sizeof(struct block) == TARRAY_LENGTH(selection->blocks));

    block *backup = buffer_data(buffer);
    size_t blockcount = TARRAY_LENGTH(selection->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        *selection->blocks.begin[i] = backup[i];
    }
}

static bool is_selected(void *data, void *element)
{
    struct blockref *ref = element;

    return is_block_selected(data, *ref);
}

static void select(void *data, void *element)
{
    struct blockref *ref = element;

    select_block(data, *ref);
}

static void deselect(void *data, void *element)
{
    struct blockref *ref = element;

    deselect_block(data, *ref);
}

static void deselect_all(void *data)
{
    deselect_all_blocks(data);
}

static pickprocs interface =
{
    is_selected,
    select,
    deselect,
    deselect_all
};

bool single_pick_block(blockselection *selection, struct blockref *ref)
{
    return single_pick(&interface, selection, ref);
}

bool multi_pick_block(blockselection *selection, struct blockref *ref)
{
    return multi_pick(&interface, selection, ref);
}
