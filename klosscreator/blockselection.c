#include "blockselection.h"
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <kloss/tarray.h>
#include <klosscreator/pick.h>
#include <assert.h>
#include <stdlib.h>

struct BlockSelection
{
    DECLARE_TARRAY(Block*, Blocks);
};

BlockSelection *CreateBlockSelection(void)
{
    return calloc(1, sizeof(BlockSelection));
}

void DestroyBlockSelection(BlockSelection *selection)
{
    if (selection)
    {
        FREE_TARRAY(selection->Blocks);
        free(selection);
    }
}

void SelectBlock(BlockSelection *selection, struct Block *block)
{
    PUSH_TARRAY(selection->Blocks, block);
}

void DeselectAllBlocks(BlockSelection *selection)
{
    CLEAR_TARRAY(selection->Blocks);
}

size_t SelectedBlockCount(BlockSelection *selection)
{
    return TARRAY_LENGTH(selection->Blocks);
}

Block **SelectedBlocks(BlockSelection *selection)
{
    return selection->Blocks.Begin;
}

void BackupBlockSelection(BlockSelection *selection, Buffer *buffer)
{
    for (Block **i = selection->Blocks.Begin; i != selection->Blocks.End; ++i)
    {
        AppendToBuffer(buffer, *i, sizeof(Block));
    }
}

void RestoreBlockSelection(BlockSelection *selection, Buffer *buffer)
{
    assert(BufferSize(buffer) / sizeof(Block) == TARRAY_LENGTH(selection->Blocks));

    Block *backup = BufferData(buffer);
    size_t blockCount = TARRAY_LENGTH(selection->Blocks);

    for (size_t i = 0; i < blockCount; ++i)
    {
        *selection->Blocks.Begin[i] = backup[i];
    }
}

static bool IsSelected(void *data, void *element)
{
    BlockSelection *selection = data;

    for (Block **i = selection->Blocks.Begin; i != selection->Blocks.End; ++i)
    {
        if (*i == element)
        {
            return true;
        }
    }

    return false;
}

static void Select(void *data, void *element)
{
    SelectBlock(data, element);
}

static void Deselect(void *data, void *element)
{
    BlockSelection *selection = data;

    for (Block **i = selection->Blocks.Begin; i != selection->Blocks.End; ++i)
    {
        if (*i == element)
        {
            ERASE_TARRAY_ITERATOR(selection->Blocks, i);
            return;
        }
    }
}

static void DeselectAll(void *data)
{
    DeselectAllBlocks(data);
}

static PickInterface interface =
{
    IsSelected,
    Select,
    Deselect,
    DeselectAll
};

bool SinglePickBlock(BlockSelection *selection, Block *pick)
{
    return SinglePick(&interface, selection, pick);
}

bool MultiPickBlock(BlockSelection *selection, Block *pick)
{
    return MultiPick(&interface, selection, pick);
}
