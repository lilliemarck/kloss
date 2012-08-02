#pragma once

#include <memory>
#include <cml/cml.h>

namespace kloss {

struct bounding_box;

using group_ptr = std::shared_ptr<class group>;

class group_instance
{
public:
    group_instance(group_ptr const& group);

    void draw() const;
    kloss::bounding_box bounding_box() const;
    kloss::bounding_box bounding_box(cml::vector3f const& parent_translation) const;
    void move_origin(cml::vector3f const& position);

private:
    cml::vector3f position_;
    group_ptr group_;
};

} // namespace kloss
