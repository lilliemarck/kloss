#include "groupinstance.h"
#include <GL/gl.h>
#include <kloss/boundingbox.h>
#include <kloss/group.h>
#include <kloss/math.h>
#include <stdlib.h>

struct GroupInstance
{
    Vec3 position_;
    Group *group_;
};

GroupInstance *CreateGroupInstance(Group *group)
{
    GroupInstance *instance = calloc(1, sizeof(GroupInstance));
    instance->group_ = RetainGroup(group);
    return instance;
}

void DestroyGroupInstance(GroupInstance *instance)
{
    if (instance)
    {
        ReleaseGroup(instance->group_);
        free(instance);
    }
}

void DrawGroupInstance(GroupInstance const *instance)
{
    glPushMatrix();
    glTranslatef(instance->position_.X, instance->position_.Y, instance->position_.Z);
    DrawGroup_(instance->group_);
    glPopMatrix();
}

BoundingBox GroupInstanceBoundingBox(GroupInstance const *instance, Vec3 const *parentPos)
{
    Vec3 totalTranslation;
    Vec3Add(&totalTranslation, parentPos, &instance->position_);
    return GroupBoundingBox(instance->group_, &totalTranslation);
}

static void TranslateBlockCallback(Block *block, void *data)
{
    TranslateBlock(block, data);
}

static void TranslateGroupInstanceCallback(GroupInstance *instance, void *data)
{
    Vec3Add(&instance->position_, &instance->position_, data);
}

void MoveGroupInstanceOrigin(GroupInstance *instance, Vec3 const *position)
{
    Vec3 translation;
    Vec3Subtract(&translation, &instance->position_, position);

    ForEachBlockInGroup(instance->group_, TranslateBlockCallback, &translation);
    ForEachGroupInstance(instance->group_, TranslateGroupInstanceCallback, &translation);

    instance->position_ = *position;
    UpdateGroupVertexArray(instance->group_);
}
