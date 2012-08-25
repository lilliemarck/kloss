#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Block;
struct Buffer;

typedef struct BlockSelection BlockSelection;

struct BlockSelection *CreateBlockSelection(void);
void DestroyBlockSelection(BlockSelection *selection);
void SelectBlock(BlockSelection *selection, struct Block *block);
void DeselectAllBlocks(BlockSelection *selection);
size_t SelectedBlockCount(BlockSelection *selection);
struct Block **SelectedBlocks(BlockSelection *Selection);

void BackupBlockSelection(BlockSelection *selection, struct Buffer *buffer);
void RestoreBlockSelection(BlockSelection *selection, struct Buffer *buffer);

bool SinglePickBlock(BlockSelection *selection, struct Block *pick);
bool MultiPickBlock(BlockSelection *selection, struct Block *pick);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
