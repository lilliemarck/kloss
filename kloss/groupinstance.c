#include "groupinstance.h"
#include <GL/gl.h>
#include <kloss/boundingbox.h>
#include <kloss/group.h>
#include <kloss/math.h>
#include <stdlib.h>

struct groupinstance
{
    vec3 position_;
    group *group_;
};

groupinstance *create_groupinstance(group *group)
{
    groupinstance *instance = calloc(1, sizeof(groupinstance));
    instance->group_ = retain_group(group);
    return instance;
}

void destroy_groupinstance(groupinstance *instance)
{
    if (instance)
    {
        release_group(instance->group_);
        free(instance);
    }
}

void draw_groupinstance(groupinstance const *instance)
{
    glPushMatrix();
    glTranslatef(instance->position_.x, instance->position_.y, instance->position_.z);
    draw_group_(instance->group_);
    glPopMatrix();
}

boundingbox groupinstance_boundingbox(groupinstance const *instance, vec3 const *parentPos)
{
    vec3 totalTranslation;
    vec3_add(&totalTranslation, parentPos, &instance->position_);
    return group_bounding_box(instance->group_, &totalTranslation);
}

static void translate_block_(block *block, void *data)
{
    translate_block(block, data);
}

static void translate_groupinstance_(groupinstance *instance, void *data)
{
    vec3_add(&instance->position_, &instance->position_, data);
}

void move_groupinstance_origin(groupinstance *instance, vec3 const *position)
{
    vec3 translation;
    vec3_subtract(&translation, &instance->position_, position);

    foreach_block(instance->group_, translate_block_, &translation);
    foreach_groupinstance(instance->group_, translate_groupinstance_, &translation);

    instance->position_ = *position;
    update_group_vertexarray(instance->group_);
}
