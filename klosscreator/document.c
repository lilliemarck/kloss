#include "document.h"
#include <kloss/boundingbox.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
#include <kloss/groupinstance.h>
#include <klosscreator/blockselection.h>
#include <assert.h>
#include <stdlib.h>

struct Document
{
    unsigned LockCount;
    Group* Group;
    BlockSelection* BlockSelection;
    Buffer* CopiedBlocks;
};

Document *CreateDocument(void)
{
    Document *doc = malloc(sizeof(Document));

    doc->LockCount = 0;
    doc->Group = CreateGroup();
    doc->BlockSelection = CreateBlockSelection();
    doc->CopiedBlocks = CreateBuffer();

    return doc;
}

void DestroyDocument(Document *doc)
{
    if (doc)
    {
        DestroyBuffer(doc->CopiedBlocks);
        DestroyBlockSelection(doc->BlockSelection);
        ReleaseGroup(doc->Group);
        free(doc);
    }
}

void LockDocument(Document *doc)
{
    ++doc->LockCount;
}

void UnlockDocument(Document *doc)
{
    assert(IsDocumentLocked(doc));
    --doc->LockCount;
}

bool IsDocumentLocked(Document *doc)
{
    return doc->LockCount;
}

void CopySelectedBlocks(Document *doc)
{
    if (IsDocumentLocked(doc) || SelectedBlockCount(doc->BlockSelection) == 0)
    {
        return;
    }

    ClearBuffer(doc->CopiedBlocks);
    BackupBlockSelection(doc->BlockSelection, doc->CopiedBlocks);
}

void PasteSelectedBlocks(Document *doc)
{
    if (IsDocumentLocked(doc) || BufferSize(doc->CopiedBlocks) == 0)
    {
        return;
    }

    DeselectAllBlocks(doc->BlockSelection);

    Block* blocks = BufferData(doc->CopiedBlocks);
    size_t blockCount = BufferSize(doc->CopiedBlocks) / sizeof(Block);

    for (size_t i = 0; i < blockCount; ++i)
    {
        Block *newBlock = CopyBlock(blocks + i);
        InsertBlocksInGroup(doc->Group, &newBlock, 1);
        SelectBlock(doc->BlockSelection, newBlock);
    }
}

void DeleteSelectedBlocks(Document *doc)
{
    if (IsDocumentLocked(doc) || SelectedBlockCount(doc->BlockSelection) == 0)
    {
        return;
    }

    Block** blocks = SelectedBlocks(doc->BlockSelection);
    size_t blockCount = SelectedBlockCount(doc->BlockSelection);

    DeleteBlocksFromGroup(doc->Group, blocks, blockCount);
    DeselectAllBlocks(doc->BlockSelection);
}

void GroupSelectedBlocks(Document *doc)
{
    if (IsDocumentLocked(doc) || SelectedBlockCount(doc->BlockSelection) == 0)
    {
        return;
    }

    Block** blocks = SelectedBlocks(doc->BlockSelection);
    size_t blockCount = SelectedBlockCount(doc->BlockSelection);

    DeleteBlocksFromGroup(doc->Group, blocks, blockCount);
    Group *newGroup = CreateGroup();
    InsertBlocksInGroup(newGroup, blocks, blockCount);
    DeselectAllBlocks(doc->BlockSelection);

    Vec3 zero = {0.0f, 0.0f, 0.0f};
    GroupInstance *instance = CreateGroupInstance(newGroup);
    BoundingBox bbox = GroupInstanceBoundingBox(instance, &zero);
    MoveGroupInstanceOrigin(instance, &bbox.Lower);

    InsertGroupInstance(doc->Group, instance);
    ReleaseGroup(newGroup);
}

Group *GetRootGroup(Document *doc)
{
    return doc->Group;
}

BlockSelection *GetBlockSelection(Document *doc)
{
    return doc->BlockSelection;
}
