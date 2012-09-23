#pragma once

#include <stdbool.h>

struct blockselection;
struct group;

typedef struct document document;

document *create_document(void);
void destroy_document(document *doc);
void lock_document(document *doc);
void unlock_document(document *doc);
bool is_document_locked(document *doc);

void copy_selected_blocks(document *doc);
void paste_copied_blocks(document *doc);
void delete_selected_blocks(document *doc);
void group_selected_blocks(document *doc);

struct group *get_root_group(document *doc);
struct blockselection *get_blockselection(document *doc);
