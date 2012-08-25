#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct PickInterface PickInterface;

struct PickInterface
{
    bool (*IsSelected)(void *data, void *element);
    void (*Select)(void *data, void *element);
    void (*Deselect)(void *data, void *element);
    void (*DeselectAll)(void *data);
};

bool SinglePick(PickInterface *interface, void *data, void *pick);
bool MultiPick(PickInterface *interface, void *data, void *pick);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
