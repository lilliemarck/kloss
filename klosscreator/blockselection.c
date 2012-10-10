#include "blockselection.h"
#include <kloss/algorithm.h>
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
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

void group_selection(struct blockselection *selection)
{
    if (TARRAY_LENGTH(selection->blocks) == 0 &&
        TARRAY_LENGTH(selection->groups) == 0)
    {
        return;
    }

    struct group *newgroup = create_group();

    block **blocks = selected_blocks(selection);
    size_t blockcount = selected_block_count(selection);

    detatch_blocks(selection->rootgroup, blocks, blockcount);
    insert_blocks(newgroup, blocks, blockcount);

    for (struct group **group = selection->groups.begin; group != selection->groups.end; ++group)
    {
        erase_group(selection->rootgroup, *group);
        insert_group(newgroup, *group);
    }

    deselect_all_blocks(selection);
    insert_group(selection->rootgroup, newgroup);
}

void ungroup_selection(struct blockselection *selection)
{
    for (struct group **i = selection->groups.begin; i != selection->groups.end; ++i)
    {
        struct group *group = *i;
        erase_group(selection->rootgroup, group);
        merge_group(selection->rootgroup, group);
    }

    CLEAR_TARRAY(selection->groups);
}

void translate_blockselection(struct blockselection *selection, vec3 translation)
{
    for (struct block **block  = selection->blocks.begin;
                        block != selection->blocks.end;
                      ++block)
    {
        translate_block(*block, &translation);
    }

    for (struct group **group  = selection->groups.begin;
                        group != selection->groups.end;
                      ++group)
    {
        vec3 grouppos = get_group_position(*group);
        vec3_add(&grouppos, &grouppos, &translation);
        set_group_position(*group, grouppos);
    }

    update_group_vertexarray(selection->rootgroup);
}

void paste_blockcopy(struct blockselection *selection, struct blockcopy *copy)
{
    deselect_all_blocks(selection);

    for (struct block *block = copy->blocks.begin; block != copy->blocks.end; ++block)
    {
        struct block *copiedblock = copy_block(block);
        insert_blocks(selection->rootgroup, &copiedblock, 1);
        PUSH_TARRAY(selection->blocks, copiedblock);
    }

    for (struct group **group = copy->groups.begin; group != copy->groups.end; ++group)
    {
        struct group *copiedgroup = copy_group(*group);
        insert_group(selection->rootgroup, copiedgroup);
        PUSH_TARRAY(selection->groups, copiedgroup);
    }
}

void delete_blockselection(struct blockselection *selection)
{
    block **blocks = selected_blocks(selection);
    size_t blockcount = selected_block_count(selection);

    delete_blocks(selection->rootgroup, blocks, blockcount);

    for (struct group **group = selection->groups.begin; group != selection->groups.end; ++group)
    {
        destroy_group(*group);
    }

    deselect_all_blocks(selection);
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

void foreach_selected_block(struct blockselection *selection, foreachblockproc proc, void *data)
{
    vec3 rootpos = get_group_position(selection->rootgroup);
    struct blockref ref = {NULL, selection->rootgroup};

    for (struct block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        ref.block = *i;
        proc(&ref, &rootpos, data);
    }

    for (struct group **i = selection->groups.begin; i != selection->groups.end; ++i)
    {
        vec3 pos = get_group_position(*i);
        vec3_add(&pos, &pos, &rootpos);
        foreach_block_in_hiearchy(*i, &rootpos, proc, data);
    }
}

struct blockcopy* create_blockcopy(blockselection* selection)
{
    struct blockcopy *copy = calloc(1, sizeof(struct blockcopy));

    for (block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        PUSH_TARRAY(copy->blocks, **i);
    }

    for (struct group **group = selection->groups.begin; group != selection->groups.end; ++group)
    {
        PUSH_TARRAY(copy->groups, copy_group(*group));
    }

    return copy;
}

void destroy_blockcopy(struct blockcopy *copy)
{
    if (copy)
    {
        for (struct group **group = copy->groups.begin; group != copy->groups.end; ++group)
        {
            destroy_group(*group);
        }

        FREE_TARRAY(copy->blocks);
        FREE_TARRAY(copy->groups);
        free(copy);
    }
}

void restore_blockcopy(blockselection *selection, struct blockcopy const *copy)
{
    assert(TARRAY_LENGTH(copy->blocks) == TARRAY_LENGTH(selection->blocks));
    assert(TARRAY_LENGTH(copy->groups) == TARRAY_LENGTH(selection->groups));

    size_t blockcount = TARRAY_LENGTH(selection->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        *selection->blocks.begin[i] = copy->blocks.begin[i];
    }

    size_t groupcount = TARRAY_LENGTH(selection->groups);

    for (size_t i = 0; i < groupcount; ++i)
    {
        vec3 pos = get_group_position(copy->groups.begin[i]);
        set_group_position(selection->groups.begin[i], pos);
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
