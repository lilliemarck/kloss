#pragma once

#include <kloss/block.h>
#include <kloss/boundingbox.h>
#include <kloss/cornerref.h>
#include <stdbool.h>
#include <stddef.h>

struct group;

typedef struct pick pick;
typedef struct viewport viewport;

struct pick
{
    struct group *group;
    struct block *block;
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
void destroy_group(struct group *group);
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
