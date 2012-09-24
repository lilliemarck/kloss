#include "document.h"
#include <kloss/boundingbox.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
#include <kloss/groupinstance.h>
#include <klosscreator/blockselection.h>
#include <stdlib.h>

struct document
{
    bool islocked;
    group* group;
    blockselection *blockselection;
    buffer *copiedblocks;
};

document *create_document(void)
{
    document *doc = malloc(sizeof(struct document));

    doc->islocked = false;
    doc->group = create_group();
    doc->blockselection = create_blockselection();
    doc->copiedblocks = create_buffer();

    return doc;
}

void destroy_document(document *doc)
{
    if (doc)
    {
        destroy_buffer(doc->copiedblocks);
        destroy_blockselection(doc->blockselection);
        release_group(doc->group);
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
        block *newBlock = copy_block(blocks + i);
        insert_blocks(doc->group, &newBlock, 1);
        select_block(doc->blockselection, newBlock);
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
    group *newgroup = create_group();
    insert_blocks(newgroup, blocks, blockcount);
    deselect_all_blocks(doc->blockselection);

    vec3 zero = {0.0f, 0.0f, 0.0f};
    groupinstance *instance = create_groupinstance(newgroup);
    boundingbox bbox = groupinstance_boundingbox(instance, &zero);
    move_groupinstance_origin(instance, &bbox.lower);

    insert_groupinstance(doc->group, instance);
    release_group(newgroup);
}

group *get_root_group(document *doc)
{
    return doc->group;
}

blockselection *get_blockselection(document *doc)
{
    return doc->blockselection;
}
