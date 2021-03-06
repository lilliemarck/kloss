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

struct group *create_group(void);
struct group *copy_group(struct group *group);
void destroy_group(struct group *group);
vec3 get_group_position(struct group const *group);
void set_group_position(struct group *group, vec3 pos);
size_t child_group_count(struct group const *group);
void insert_blocks(struct group *group, block **blocks, size_t count);
void delete_blocks(struct group *group, block **blocks, size_t count);
void detatch_blocks(struct group *group, block **blocks, size_t count);
block **get_blocks(struct group *group);
size_t block_count(struct group const *group);
void insert_group(struct group *group, struct group *child);
void erase_group(struct group *group, struct group *child);
pick pick_block(struct group *group, ray const *ray);
bool pick_vertex(struct group *group, mat4 const *model, mat4 const *projection, viewport const *viewport, vec2 const *mouse, cornerref *ref);
void update_group_vertexarray(struct group *group);
void draw_group(struct group const *group);
void move_group_origin(struct group *instance, struct vec3 const *position);
void foreach_block_in_hiearchy(
    struct group *group,
    struct vec3 const *parentpos,
    void (*proc)(struct blockref*, struct vec3 const *pos, void*),
    void *data);

/**
 * Merges a group into another and destroys the original group.
 */
void merge_group(struct group *group, struct group *child);

/**
 * Returns a pointer to the child group that contains the descendant or that
 * is the descendant itself. Returns NULL if descendant is not a descendant of
 * group.
 */
struct group *get_child_by_descendant(struct group const *group, struct group *descendant);
