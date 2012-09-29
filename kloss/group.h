#pragma once

#include <kloss/block.h>
#include <kloss/boundingbox.h>
#include <kloss/cornerref.h>
#include <stdbool.h>
#include <stddef.h>

struct group;

typedef struct pick pick;
typedef struct viewport viewport;

/**
 * Reference to a block and the group that the block is part of.
 * This is needed because blocks doesn't have a pointer their group.
 */
struct blockref
{
    struct block *block;
    struct group *group;
};

struct pick
{
    struct blockref blockref;
    struct triangle triangle;
    struct vec3 intersection;
};

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

struct group *create_group(struct group *parent);
void destroy_group(struct group *group);
vec3 get_group_position(struct group const *group);
void insert_blocks(struct group *group, block **blocks, size_t count);
void delete_blocks(struct group *group, block **blocks, size_t count);
void detatch_blocks(struct group *group, block **blocks, size_t count);
void insert_group(struct group *group, struct group *child);
pick pick_block(struct group *group, ray const *ray);
bool pick_vertex(struct group *group, mat4 const *model, mat4 const *projection, viewport const *viewport, vec2 const *mouse, cornerref *ref);
void update_group_vertexarray(struct group *group);
void draw_group(struct group const *group);
struct boundingbox group_boundingbox(struct group const *instance);
void move_group_origin(struct group *instance, struct vec3 const *position);
void foreach_block_in_hiearchy(struct group *group, void (*proc)(struct blockref*, void*), void *data);

/**
 * Merges this group into the parent group and destroy this group.
 */
void merge_group_into_parent(struct group *group);

/**
 * Returns a pointer to the child group that contains the descendant or that
 * is the descendant itself. Returns NULL if descendant is not a descendant of
 * group.
 */
struct group *get_child_by_descendant(struct group const *group, struct group *descendant);
