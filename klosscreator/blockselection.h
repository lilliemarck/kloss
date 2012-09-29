#pragma once

#include <stdbool.h>
#include <stddef.h>

struct block;
struct blockref;
struct buffer;
struct group;

typedef struct blockselection blockselection;

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

void backup_blockselection(blockselection *selection, struct buffer *buffer);
void restore_blockselection(blockselection *selection, struct buffer *buffer);

bool single_pick_block(blockselection *selection, struct blockref *ref);
bool multi_pick_block(blockselection *selection, struct blockref *ref);
