#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct BoundingBox;
struct Group;
struct Vec3;

typedef struct GroupInstance GroupInstance;

GroupInstance *CreateGroupInstance(struct Group *group);
void DestroyGroupInstance(GroupInstance *instance);
void DrawGroupInstance(GroupInstance const *instance);
struct BoundingBox GroupInstanceBoundingBox(GroupInstance const *instance, struct Vec3 const* parentPos);
void MoveGroupInstanceOrigin(GroupInstance *instance, struct Vec3 const* position);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
