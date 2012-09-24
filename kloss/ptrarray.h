#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct PtrArray PtrArray;

PtrArray *CreatePtrArray(void);
void DestroyPtrArray(PtrArray *array);
void PushPtrArray(PtrArray *array, void *element);
void *PopPtrArray(PtrArray *array);
void *PtrArrayBack(PtrArray *array);
void *GetPtrArray(PtrArray *array, size_t index);
void PutPtrArray(PtrArray *array, size_t index, void *element);
void ErasePtrArray(PtrArray *array, size_t index);
void RemovePtrArray(PtrArray *array, void const *element);
void ClearPtrArray(PtrArray *array);
size_t PtrArrayCount(const PtrArray *array);
