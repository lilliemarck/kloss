#pragma once

#include <stdbool.h>
#include <stddef.h>

struct block;
struct buffer;

typedef struct blockselection blockselection;

struct blockselection *create_blockselection(void);
void destroy_blockselection(blockselection *selection);
void select_block(blockselection *selection, struct block *block);
void deselect_all_blocks(blockselection *selection);
size_t selected_block_count(blockselection *selection);
struct block **selected_blocks(blockselection *selection);

void backup_blockselection(blockselection *selection, struct buffer *buffer);
void restore_blockselection(blockselection *selection, struct buffer *buffer);

bool single_pick_block(blockselection *selection, struct block *pick);
bool multi_pick_block(blockselection *selection, struct block *pick);
