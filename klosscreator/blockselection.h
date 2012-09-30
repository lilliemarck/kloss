#pragma once

#include <kloss/tarray.h>
#include <stdbool.h>
#include <stddef.h>

struct block;
struct blockref;
struct buffer;
struct group;
struct vec3;

struct blockcopy
{
    DECLARE_TARRAY(struct block, blocks);
    DECLARE_TARRAY(struct group*, groups);
};

typedef struct blockselection blockselection;
typedef void (*foreachblockproc)(struct blockref *ref, void *data);

/**
 * Create a block selection with the given group as the root group.
 */
struct blockselection *create_blockselection(struct group *rootgroup);
void destroy_blockselection(blockselection *selection);

/**
 * Selects the block or the group that the block is part of. The selection is
 * left unchanged if the block is part of of an ancestor or unrelated group.
 */
void select_block(blockselection *selection, struct blockref ref);

/**
 * Deselects the block or the group that the block is part of.
 */
void deselect_block(blockselection *selection, struct blockref ref);
void deselect_all_blocks(blockselection *selection);
size_t selected_block_count(blockselection *selection);
struct block **selected_blocks(blockselection *selection);
void foreach_selected_block(struct blockselection *selection, foreachblockproc proc, void *data);

/**
 * Merges selected child groups into the root group.
 */
void ungroup_selection(struct blockselection *selection);
void translate_blockselection(struct blockselection *selection, struct vec3 translation);
void paste_blockcopy(struct blockselection *selection, struct blockcopy *copy);

struct blockcopy *create_blockcopy(blockselection *selection);
void destroy_blockcopy(struct blockcopy *copy);
/**
 * Restores an earlier copy made from this block selection. The number of
 * blocks and groups in the copy must match what is in the selection.
 */
void restore_blockcopy(blockselection *selection, struct blockcopy const *copy);

bool single_pick_block(blockselection *selection, struct blockref *ref);
bool multi_pick_block(blockselection *selection, struct blockref *ref);
