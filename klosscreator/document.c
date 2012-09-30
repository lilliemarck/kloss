#include "document.h"
#include <kloss/boundingbox.h>
#include <kloss/group.h>
#include <klosscreator/blockselection.h>
#include <stdlib.h>

struct document
{
    bool islocked;
    struct group* group;
    blockselection *blockselection;
    struct blockcopy *copiedblocks;
};

document *create_document(void)
{
    document *doc = calloc(1, sizeof(struct document));

    doc->group = create_group();
    doc->blockselection = create_blockselection(doc->group);

    return doc;
}

void destroy_document(document *doc)
{
    if (doc)
    {
        destroy_blockcopy(doc->copiedblocks);
        destroy_blockselection(doc->blockselection);
        destroy_group(doc->group);
        free(doc);
    }
}

void lock_document(document *doc)
{
    doc->islocked = true;
}

void unlock_document(document *doc)
{
    doc->islocked = false;
}

bool is_document_locked(document *doc)
{
    return doc->islocked;
}

void copy_selected_blocks(document *doc)
{
    if (is_document_locked(doc))
    {
        return;
    }

    destroy_blockcopy(doc->copiedblocks);
    doc->copiedblocks = create_blockcopy(doc->blockselection);
}

void paste_copied_blocks(document *doc)
{
    if (is_document_locked(doc) || !doc->copiedblocks)
    {
        return;
    }

    paste_blockcopy(doc->blockselection, doc->copiedblocks);
}

void delete_selected_blocks(document *doc)
{
    if (is_document_locked(doc))
    {
        return;
    }

    delete_blockselection(doc->blockselection);
}

void group_selected_blocks(document *doc)
{
    if (is_document_locked(doc) || selected_block_count(doc->blockselection) == 0)
    {
        return;
    }

    block **blocks = selected_blocks(doc->blockselection);
    size_t blockcount = selected_block_count(doc->blockselection);

    detatch_blocks(doc->group, blocks, blockcount);

    struct group *newgroup = create_group();
    insert_blocks(newgroup, blocks, blockcount);
    deselect_all_blocks(doc->blockselection);

    boundingbox bbox = group_boundingbox(newgroup);
    move_group_origin(newgroup, &bbox.lower);

    insert_group(doc->group, newgroup);
}

void ungroup_selected_blocks(document *doc)
{
    if (is_document_locked(doc))
    {
        return;
    }

    ungroup_selection(doc->blockselection);
}

struct group *get_root_group(document *doc)
{
    return doc->group;
}

blockselection *get_blockselection(document *doc)
{
    return doc->blockselection;
}
