#include "blockselection.h"
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <kloss/tarray.h>
#include <klosscreator/pick.h>
#include <assert.h>
#include <stdlib.h>

struct blockselection
{
    DECLARE_TARRAY(block*, blocks);
};

blockselection *create_blockselection(void)
{
    return calloc(1, sizeof(blockselection));
}

void destroy_blockselection(blockselection *selection)
{
    if (selection)
    {
        FREE_TARRAY(selection->blocks);
        free(selection);
    }
}

void select_block(blockselection *selection, struct block *block)
{
    PUSH_TARRAY(selection->blocks, block);
}

void deselect_all_blocks(blockselection *selection)
{
    CLEAR_TARRAY(selection->blocks);
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
        append_buffer(buffer, *i, sizeof(block));
    }
}

void restore_blockselection(blockselection *selection, buffer *buffer)
{
    assert(buffer_size(buffer) / sizeof(block) == TARRAY_LENGTH(selection->blocks));

    block *backup = buffer_data(buffer);
    size_t blockcount = TARRAY_LENGTH(selection->blocks);

    for (size_t i = 0; i < blockcount; ++i)
    {
        *selection->blocks.begin[i] = backup[i];
    }
}

static bool is_selected(void *data, void *element)
{
    blockselection *selection = data;

    for (block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        if (*i == element)
        {
            return true;
        }
    }

    return false;
}

static void select(void *data, void *element)
{
    select_block(data, element);
}

static void deselect(void *data, void *element)
{
    blockselection *selection = data;

    for (block **i = selection->blocks.begin; i != selection->blocks.end; ++i)
    {
        if (*i == element)
        {
            ERASE_TARRAY_ITERATOR(selection->blocks, i);
            return;
        }
    }
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

bool single_pick_block(blockselection *selection, block *pick)
{
    return single_pick(&interface, selection, pick);
}

bool multi_pick_block(blockselection *selection, block *pick)
{
    return multi_pick(&interface, selection, pick);
}
