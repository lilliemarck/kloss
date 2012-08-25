#pragma once

#include <stdbool.h>

struct BlockSelection;
struct Group;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Document Document;

Document *CreateDocument(void);
void DestroyDocument(Document *doc);
void LockDocument(Document *doc);
void UnlockDocument(Document *doc);
bool IsDocumentLocked(Document *doc);

void CopySelectedBlocks(Document *doc);
void PasteSelectedBlocks(Document *doc);
void DeleteSelectedBlocks(Document *doc);
void GroupSelectedBlocks(Document *doc);

struct Group *GetRootGroup(Document *doc);
struct BlockSelection *GetBlockSelection(Document *doc);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
