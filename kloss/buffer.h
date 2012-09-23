#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct buffer buffer;

buffer *create_buffer(void);
void destroy_buffer(buffer *buffer);
void append_buffer(buffer *buffer, void const *data, size_t size);
void erase_buffer(buffer *buffer, size_t index, size_t size);
void clear_buffer(buffer *buffer);
size_t buffer_size(buffer *buffer);
void *buffer_data(buffer *buffer);
