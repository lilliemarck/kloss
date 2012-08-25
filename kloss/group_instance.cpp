#include "group_instance.hpp"
#include <GL/gl.h>
#include <kloss/bounding_box.hpp>
#include <kloss/group.hpp>

namespace kloss {

group_instance::group_instance(group_ptr const& group)
    : position_{0.0f, 0.0f, 0.0f}
    , group_{group}
{
    assert(group);
}

void group_instance::draw() const
{
    glPushMatrix();
    glTranslatef(position_.X, position_.Y, position_.Z);

    group_->draw();

    glPopMatrix();
}

bounding_box group_instance::bounding_box() const
{
    return bounding_box({0.0f, 0.0f, 0.0f});
}

bounding_box group_instance::bounding_box(Vec3 const& parent_translation) const
{
    Vec3 total_translation;
    Vec3Add(&total_translation, &parent_translation, &position_);
    return group_->bounding_box(total_translation);
}

void group_instance::move_origin(Vec3 const& position)
{
    Vec3 translation;
    Vec3Subtract(&translation, &position_, &position);

    group_->for_each_block([&](block_ptr const& block)
    {
        for (corner& corner : *block)
        {
            translate(corner, translation);
        }
    });

    group_->for_each_group_instance([&](kloss::group_instance& group_instance)
    {
        Vec3Add(&group_instance.position_, &group_instance.position_, &translation);
    });

    position_ = position;
    group_->update_vertex_array();
}

} // namespace kloss
