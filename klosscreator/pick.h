#pragma once

#include <stdbool.h>

typedef struct pickprocs pickprocs;

struct pickprocs
{
    bool (*is_selected)(void *data, void *element);
    void (*select)(void *data, void *element);
    void (*deselect)(void *data, void *element);
    void (*deselect_all)(void *data);
};

bool single_pick(pickprocs *interface, void *data, void *pick);
bool multi_pick(pickprocs *interface, void *data, void *pick);
