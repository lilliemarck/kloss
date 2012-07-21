#pragma once

#include <cml/cml.h>

namespace kloss {

struct camera
{
    camera();

    cml::vector3f position;
    float yaw;
    float pitch;
};

cml::matrix44f_c world_transform(camera const& camera);

} // namespace kloss
