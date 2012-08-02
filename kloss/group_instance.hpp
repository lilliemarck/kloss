#pragma once

#include <memory>
#include <cml/cml.h>

namespace kloss {

using group_ptr = std::shared_ptr<class group>;

class group_instance
{
public:
    group_instance(group_ptr const& group);

    void draw() const;

private:
    cml::vector3f position_;
    group_ptr group_;
};

} // namespace kloss
