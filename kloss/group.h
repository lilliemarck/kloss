#pragma once

#include <kloss/block.h>
#include <kloss/boundingbox.h>
#include <kloss/cornerref.h>
#include <stdbool.h>
#include <stddef.h>

struct groupinstance;

typedef struct pick pick;
typedef struct viewport viewport;
typedef struct group group;

struct pick
{
    block *block;
    triangle triangle;
    vec3 intersection;
};

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

group *create_group(void);
group *retain_group(group *group);
void release_group(group *group);
void insert_blocks(group *group, block **blocks, size_t count);
void delete_blocks(group *group, block **blocks, size_t count);
void detatch_blocks(group *group, block **blocks, size_t count);
void foreach_block(group *group, void (function)(block*,void*), void *data);
void insert_groupinstance(group *group, struct groupinstance *instance);
void foreach_groupinstance(group *group, void (function)(struct groupinstance*,void*), void *data);
pick pick_group_block(group const *group, ray const *ray);
bool pick_group_vertex(group const *group, mat4 const *model, mat4 const *projection, viewport const *viewport, vec2 const *mouse, cornerref *ref);
boundingbox group_bounding_box(group const *group, vec3 const *translation);
void update_group_vertexarray(group *group);
void draw_group(group const *group);
void draw_group_(group const *group);
