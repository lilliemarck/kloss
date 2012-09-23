#pragma once

#include <stdbool.h>
#include <stddef.h>

struct buffer;
struct cornerref;

typedef struct cornerselection cornerselection;

cornerselection *create_cornerselection(void);
void destroy_cornerselection(cornerselection *selection);
size_t selected_corner_count(cornerselection *selection);
struct cornerref *selected_corners(cornerselection *selection);

void backup_cornerselection(cornerselection *selection, struct buffer *buffer);
void restore_cornerselection(cornerselection *selection, struct buffer *buffer);

bool single_pick_corner(cornerselection *selection, struct cornerref *pick);
bool multi_pick_corner(cornerselection *selection, struct cornerref *pick);
