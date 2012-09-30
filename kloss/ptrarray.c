#include "ptrarray.h"
#include <kloss/algorithm.h>
#include <stdlib.h>
#include <string.h>

struct ptrarray
{
    void **begin;
    void **end;
    void **capacity;
};

ptrarray* create_ptrarray(void)
{
    return calloc(1, sizeof(struct ptrarray));
}

void destroy_ptrarray(ptrarray *array)
{
    if (array)
    {
        free(array->begin);
        free(array);
    }
}

static void grow_capacity(ptrarray *array, void **end)
{
    if (end > array->capacity)
    {
        size_t length   = array->end - array->begin;
        size_t capacity = array->capacity - array->begin;

        capacity = maxz(capacity << 1, end - array->begin);

        array->begin    = realloc(array->begin, capacity * sizeof(void*));
        array->end      = array->begin + length;
        array->capacity = array->begin + capacity;
    }
}

void push_ptrarray(ptrarray *array, void *object)
{
    grow_capacity(array, array->end + 1);
    *array->end++ = object;
}

void *pop_ptrarray(ptrarray *array)
{
    return *--array->end;
}

void *ptrarray_back(ptrarray *array)
{
    return *(array->end - 1);
}

void *get_ptrarray(ptrarray *array, size_t index)
{
    return array->begin[index];
}

void put_ptrarray(ptrarray *array, size_t index, void *element)
{
    array->begin[index] = element;
}

void erase_ptrarray(ptrarray *array, size_t index)
{
    memcpy(array->begin + index,
           array->begin + index + 1,
           sizeof(void*) * (--array->end - array->begin - index));
}

void remove_ptrarray(ptrarray *array, void const *element)
{
    size_t count = ptrarray_count(array);
    size_t i     = 0;

    while (i < count)
    {
        if (array->begin[i] == element)
        {
            erase_ptrarray(array, i);
            --count;
        }
        else
        {
            ++i;
        }
    }
}

void append_ptrarray(ptrarray *array, ptrarray const *from)
{
    size_t count = ptrarray_count(from);
    grow_capacity(array, array->end + count);
    memcpy(array->end, from->begin, count * sizeof(void*));
    array->end += count;
}

void clear_ptrarray(ptrarray *array)
{
    array->end = array->begin;
}

size_t ptrarray_count(ptrarray const *array)
{
    return array->end - array->begin;
}
