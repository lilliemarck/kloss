#pragma once

#include <stdlib.h>
#include <string.h>

#define DECLARE_TARRAY(T, name) \
struct                          \
{                               \
    T *begin;                   \
    T *end;                     \
    T *capacity;                \
} name

#define INIT_TARRAY(array)   \
{                            \
    (array).begin = NULL;    \
    (array).end = NULL;      \
    (array).capacity = NULL; \
}

#define INIT_TARRAY_CAPACITY(array, capacity)                  \
{                                                              \
    (array).begin = malloc(sizeof(*(array).begin) * capacity); \
    (array).end = (array).begin;                               \
    (array).capacity = (array).begin + capacity;               \
}

#define FREE_TARRAY(array) \
{                          \
    free((array).begin);   \
}

#define PUSH_TARRAY(array, element)                                                \
{                                                                                  \
    if ((array).end == (array).capacity)                                           \
    {                                                                              \
        size_t length = (array).end - (array).begin;                               \
        size_t capacity = (length + 1) << 1;                                       \
        (array).begin = realloc((array).begin, sizeof(*(array).begin) * capacity); \
        (array).end = (array).begin + length;                                      \
        (array).capacity = (array).begin + capacity;                               \
    }                                                                              \
    *(array).end++ = (element);                                                    \
}

#define ERASE_TARRAY_INDEX(array, index)                                        \
{                                                                               \
    memcpy((array).begin + (index),                                             \
           (array).begin + (index) + 1,                                         \
           sizeof(*(array).begin) * (--(array).end - (array).begin - (index))); \
}

/**
 * Erases an element given a pointer of the correct type to that element.
 */
#define ERASE_TARRAY_ITERATOR(array, iterator)                     \
{                                                                  \
    memcpy(iterator,                                               \
           iterator + 1,                                           \
           sizeof(*(array).begin) * (--(array).end - (iterator))); \
}

#define CLEAR_TARRAY(array) { (array).end = (array).begin; }

#define TARRAY_LENGTH(array) ((array).end - (array).begin)
