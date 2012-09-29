#include "document.h"
#include <kloss/boundingbox.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
#include <klosscreator/blockselection.h>
#include <stdlib.h>

struct document
{
    bool islocked;
    struct group* group;
    blockselection *blockselection;
    buffer *copiedblocks;
};

document *create_document(void)
{
    document *doc = malloc(sizeof(struct document));

    doc->islocked = false;
    doc->group = create_group(NULL);
    doc->blockselection = create_blockselection(doc->group);
    doc->copiedblocks = create_buffer();

    return doc;
}

void destroy_document(document *doc)
{
    if (doc)
    {
        destroy_buffer(doc->copiedblocks);
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
    if (is_document_locked(doc) || selected_block_count(doc->blockselection) == 0)
    {
        return;
    }

    clear_buffer(doc->copiedblocks);
    backup_blockselection(doc->blockselection, doc->copiedblocks);
}

void paste_copied_blocks(document *doc)
{
    if (is_document_locked(doc) || buffer_size(doc->copiedblocks) == 0)
    {
        return;
    }

    deselect_all_blocks(doc->blockselection);

    block* blocks = buffer_data(doc->copiedblocks);
    size_t blockcount = buffer_size(doc->copiedblocks) / sizeof(struct block);

    for (size_t i = 0; i < blockcount; ++i)
    {
        struct blockref ref = {copy_block(blocks + i), doc->group};

        insert_blocks(ref.group, &ref.block, 1);
        select_block(doc->blockselection, ref);
    }
}

void delete_selected_blocks(document *doc)
{
    if (is_document_locked(doc) || selected_block_count(doc->blockselection) == 0)
    {
        return;
    }

    block **blocks = selected_blocks(doc->blockselection);
    size_t blockcount = selected_block_count(doc->blockselection);

    delete_blocks(doc->group, blocks, blockcount);
    deselect_all_blocks(doc->blockselection);
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

    struct group *newgroup = create_group(doc->group);
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
