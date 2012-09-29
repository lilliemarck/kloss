#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct ptrarray ptrarray;

ptrarray *create_ptrarray(void);
void destroy_ptrarray(ptrarray *array);
void push_ptrarray(ptrarray *array, void *element);
void *pop_ptrarray(ptrarray *array);
void *ptrarray_back(ptrarray *array);
void *get_ptrarray(ptrarray *array, size_t index);
void put_ptrarray(ptrarray *array, size_t index, void *element);
void erase_ptrarray(ptrarray *array, size_t index);
void remove_ptrarray(ptrarray *array, void const *element);
void append_ptrarray(ptrarray *array, ptrarray const *from);
void clear_ptrarray(ptrarray *array);
size_t ptrarray_count(const ptrarray *array);
