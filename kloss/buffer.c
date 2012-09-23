#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

struct buffer
{
    uint8_t *begin;
    uint8_t *end;
    uint8_t *capacity;
};

buffer *create_buffer(void)
{
    return calloc(1, sizeof(buffer));
}

void destroy_buffer(buffer *buffer)
{
    if (buffer)
    {
        free(buffer->begin);
        free(buffer);
    }
}

static size_t Max(size_t x, size_t y)
{
    return x < y ? y : x;
}

static void GrowCapacity(buffer *buffer, uint8_t *end)
{
    if (end > buffer->capacity)
    {
        size_t length   = buffer->end - buffer->begin;
        size_t capacity = buffer->capacity - buffer->begin;

        capacity = Max(capacity << 1, end - buffer->begin);

        buffer->begin    = realloc(buffer->begin, capacity);
        buffer->end      = buffer->begin + length;
        buffer->capacity = buffer->begin + capacity;
    }
}

void append_buffer(buffer *buffer, void const *data, size_t size)
{
    GrowCapacity(buffer, buffer->end + size);
    memcpy(buffer->end, data, size);
    buffer->end += size;
}

void erase_buffer(buffer *buffer, size_t index, size_t size)
{
    uint8_t *first = buffer->begin + index;
    memcpy(first, first + size, buffer_size(buffer) - size);
    buffer->end -= size;
}

void clear_buffer(buffer *buffer)
{
    buffer->end = buffer->begin;
}

size_t buffer_size(buffer *buffer)
{
    return buffer->end - buffer->begin;
}

void *buffer_data(buffer *buffer)
{
    return buffer->begin;
}
