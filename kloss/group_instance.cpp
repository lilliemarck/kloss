#include "group_instance.hpp"
#include <GL/gl.h>
#include <kloss/group.hpp>

namespace kloss {

group_instance::group_instance(group_ptr const& group)
    : position_(0.0f, 0.0f, 0.0f)
    , group_(group)
{
    assert(group);
}

void group_instance::draw() const
{
    glPushMatrix();
    glTranslatef(position_[0], position_[1], position_[2]);

    group_->draw();

    glPopMatrix();
}

} // namespace kloss
