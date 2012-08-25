#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Block;
struct Corner;
struct Vec3;

enum CornerRefFlags
{
    CORNER_REF_TOP = 1,
    CORNER_REF_BOTTOM = 2
};

typedef enum CornerRefFlags CornerRefFlags;
typedef struct CornerRef CornerRef;

struct CornerRef
{
    struct Corner* Corner;
    uint8_t Flags;
};

void MakeCornerRefs(CornerRef refs[4], struct Block *block);
void TranslateCornerRef(CornerRef *ref, struct Vec3 const *units);

/**
 * Return the position of either the top or bottom vertex depending on which
 * flag is set. Should not be used when both flags are set.
 */
struct Vec3 CornerRefPosition(CornerRef const *ref);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
