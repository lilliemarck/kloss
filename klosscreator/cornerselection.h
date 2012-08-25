#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Buffer;
struct CornerRef;

typedef struct CornerSelection CornerSelection;

CornerSelection *CreateCornerSelection(void);
void DestroyCornerSelection(CornerSelection *selection);
size_t SelectedCornerCount(CornerSelection *selection);
struct CornerRef *SelectedCorners(CornerSelection *selection);

void BackupCornerSelection(CornerSelection *selection, struct Buffer *buffer);
void RestoreCornerSelection(CornerSelection *selection, struct Buffer *buffer);

bool SinglePickCorner(CornerSelection *selection, struct CornerRef *pick);
bool MultiPickCorner(CornerSelection *selection, struct CornerRef *pick);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
