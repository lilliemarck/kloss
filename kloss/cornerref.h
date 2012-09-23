#pragma once

#include <stdint.h>

struct block;
struct corner;
struct vec3;

enum cornerrefflags
{
    CORNERREF_TOP = 1,
    CORNERREF_BOTTOM = 2
};

typedef enum cornerrefflags cornerrefflags;
typedef struct cornerref cornerref;

struct cornerref
{
    struct corner* corner;
    uint8_t flags;
};

void make_cornerrefs(cornerref refs[4], struct block *block);
void translate_cornerref(cornerref *ref, struct vec3 const *units);

/**
 * Return the position of either the top or bottom vertex depending on which
 * flag is set. Should not be used when both flags are set.
 */
struct vec3 cornerref_position(cornerref const *ref);
