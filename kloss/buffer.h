#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Buffer Buffer;

Buffer *CreateBuffer(void);
void DestroyBuffer(Buffer *buffer);
void AppendToBuffer(Buffer *buffer, void const *data, size_t size);
void EraseBuffer(Buffer *buffer, size_t index, size_t size);
void ClearBuffer(Buffer *buffer);
size_t BufferSize(Buffer *buffer);
void *BufferData(Buffer *buffer);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
