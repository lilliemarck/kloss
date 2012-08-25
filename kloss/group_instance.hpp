#pragma once

#include <memory>
#include <kloss/math.h>

namespace kloss {

struct bounding_box;

using group_ptr = std::shared_ptr<class group>;

class group_instance
{
public:
    group_instance(group_ptr const& group);

    void draw() const;
    kloss::bounding_box bounding_box() const;
    kloss::bounding_box bounding_box(Vec3 const& parent_translation) const;
    void move_origin(Vec3 const& position);

private:
    Vec3 position_;
    group_ptr group_;
};

} // namespace kloss
