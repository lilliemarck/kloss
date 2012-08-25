#pragma once

#include <stdlib.h>
#include <string.h>

#define DECLARE_TARRAY(T, name) \
struct                          \
{                               \
    T *Begin;                   \
    T *End;                     \
    T *Capacity;                \
} name

#define INIT_TARRAY(array)   \
{                            \
    (array).Begin = NULL;    \
    (array).End = NULL;      \
    (array).Capacity = NULL; \
}

#define INIT_TARRAY_CAPACITY(array, capacity)                  \
{                                                              \
    (array).Begin = malloc(sizeof(*(array).Begin) * capacity); \
    (array).End = (array).Begin;                               \
    (array).Capacity = (array).Begin + capacity;               \
}

#define FREE_TARRAY(array) \
{                          \
    free((array).Begin);   \
}

#define PUSH_TARRAY(array, element)                                                \
{                                                                                  \
    if ((array).End == (array).Capacity)                                           \
    {                                                                              \
        size_t length = (array).End - (array).Begin;                               \
        size_t capacity = (length + 1) << 1;                                       \
        (array).Begin = realloc((array).Begin, sizeof(*(array).Begin) * capacity); \
        (array).End = (array).Begin + length;                                      \
        (array).Capacity = (array).Begin + capacity;                               \
    }                                                                              \
    *(array).End++ = (element);                                                    \
}

#define ERASE_TARRAY_INDEX(array, index)                                        \
{                                                                               \
    memcpy((array).Begin + (index),                                             \
           (array).Begin + (index) + 1,                                         \
           sizeof(*(array).Begin) * (--(array).End - (array).Begin - (index))); \
}

/**
 * Erases an element given a pointer of the correct type to that element.
 */
#define ERASE_TARRAY_ITERATOR(array, iterator)                     \
{                                                                  \
    memcpy(iterator,                                               \
           iterator + 1,                                           \
           sizeof(*(array).Begin) * (--(array).End - (iterator))); \
}

#define CLEAR_TARRAY(array) { (array).End = (array).Begin; }

#define TARRAY_LENGTH(array) ((array).End - (array).Begin)
