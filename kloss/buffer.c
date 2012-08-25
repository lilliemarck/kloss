#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

struct Buffer
{
    uint8_t *Begin;
    uint8_t *End;
    uint8_t *Capacity;
};

Buffer *CreateBuffer(void)
{
    return calloc(1, sizeof(Buffer));
}

void DestroyBuffer(Buffer *buffer)
{
    if (buffer)
    {
        free(buffer->Begin);
        free(buffer);
    }
}

static size_t Max(size_t x, size_t y)
{
    return x < y ? y : x;
}

static void GrowCapacity(Buffer *buffer, uint8_t *end)
{
    if (end > buffer->Capacity)
    {
        size_t length   = buffer->End - buffer->Begin;
        size_t capacity = buffer->Capacity - buffer->Begin;

        capacity = Max(capacity << 1, end - buffer->Begin);

        buffer->Begin    = realloc(buffer->Begin, capacity);
        buffer->End      = buffer->Begin + length;
        buffer->Capacity = buffer->Begin + capacity;
    }
}

void AppendToBuffer(Buffer *buffer, void const *data, size_t size)
{
    GrowCapacity(buffer, buffer->End + size);
    memcpy(buffer->End, data, size);
    buffer->End += size;
}

void EraseBuffer(Buffer *buffer, size_t index, size_t size)
{
    uint8_t *first = buffer->Begin + index;
    memcpy(first, first + size, BufferSize(buffer) - size);
    buffer->End -= size;
}

void ClearBuffer(Buffer *buffer)
{
    buffer->End = buffer->Begin;
}

size_t BufferSize(Buffer *buffer)
{
    return buffer->End - buffer->Begin;
}

void *BufferData(Buffer *buffer)
{
    return buffer->Begin;
}
