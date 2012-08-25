#include "ptrarray.h"
#include <stdlib.h>
#include <string.h>

struct PtrArray
{
    void **Begin;
    void **End;
    void **Capacity;
};

PtrArray* CreatePtrArray(void)
{
    return calloc(1, sizeof(PtrArray));
}

void DestroyPtrArray(PtrArray *array)
{
    if (array)
    {
        free(array->Begin);
        free(array);
    }
}

void PushPtrArray(PtrArray *array, void *object)
{
    if (array->End == array->Capacity)
    {
        size_t length   = array->End - array->Begin;
        size_t capacity = (length + 1) << 1;
        array->Begin    = realloc(array->Begin, capacity * sizeof(void*));
        array->End      = array->Begin + length;
        array->Capacity = array->Begin + capacity;
    }

    *array->End++ = object;
}

void *PopPtrArray(PtrArray *array)
{
    return *--array->End;
}

void *PtrArrayBack(PtrArray *array)
{
    return *(array->End - 1);
}

void *GetPtrArray(PtrArray *array, size_t index)
{
    return array->Begin[index];
}

void PutPtrArray(PtrArray *array, size_t index, void *element)
{
    array->Begin[index] = element;
}

void ErasePtrArray(PtrArray *array, size_t index)
{
    memcpy(array->Begin + index,
           array->Begin + index + 1,
           sizeof(void*) * (--array->End - array->Begin - index));
}

void RemovePtrArray(PtrArray *array, void const *element)
{
    size_t count = PtrArrayCount(array);
    size_t i     = 0;

    while (i < count)
    {
        if (array->Begin[i] == element)
        {
            ErasePtrArray(array, i);
            --count;
        }
        else
        {
            ++i;
        }
    }
}

void ClearPtrArray(PtrArray *array)
{
    array->End = array->Begin;
}

size_t PtrArrayCount(PtrArray const *array)
{
    return array->End - array->Begin;
}
