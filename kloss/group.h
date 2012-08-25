#pragma once

#include <kloss/block.h>
#include <kloss/boundingbox.h>
#include <kloss/cornerref.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct GroupInstance;

typedef struct Pick Pick;
typedef struct Viewport Viewport;
typedef struct Group Group;

struct Pick
{
    Block *block;
    Triangle triangle;
    Vec3 intersection;
};

struct Viewport
{
    int x;
    int y;
    int width;
    int height;
};

Group *CreateGroup(void);
Group *RetainGroup(Group *group);
void ReleaseGroup(Group *group);
void InsertBlocksInGroup(Group *group, Block **blocks, size_t count);
void DeleteBlocksFromGroup(Group *group, Block **blocks, size_t count);
void DetatchBlocksFromGroup(Group *group, Block **blocks, size_t count);
void ForEachBlockInGroup(Group *group, void (function)(Block*,void*), void *data);
void InsertGroupInstance(Group *group, struct GroupInstance *instance);
void ForEachGroupInstance(Group *group, void (function)(struct GroupInstance*,void*), void *data);
Pick PickGroupBlock(Group const *group, Ray const *ray);
bool PickGroupVertex(Group const *group, Mat4 const *model, Mat4 const *projection, Viewport const *viewport, Vec2 const *mouse, CornerRef *ref);
BoundingBox GroupBoundingBox(Group const *group, Vec3 const *translation);
void UpdateGroupVertexArray(Group *group);
void DrawGroup(Group const *group);
void DrawGroup_(Group const *group);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
