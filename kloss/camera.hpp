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

cml::vector3f forward_vector(camera const& camera);
cml::vector3f right_vector(camera const& camera);
cml::matrix44f_c rotation_matrix(camera const& camera);
cml::matrix44f_c world_transform(camera const& camera);

} // namespace kloss
