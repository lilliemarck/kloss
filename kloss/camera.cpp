#include "camera.hpp"

namespace kloss {

camera::camera()
    : position(0.0f, 0.0f, 0.0f)
    , yaw(0.0f)
    , pitch(0.0f)
{
}

cml::matrix44f_c world_transform(camera const& camera)
{
    cml::matrix44f_c matrix;
    cml::matrix_rotation_world_x(matrix, camera.pitch);
    cml::matrix_rotate_about_world_y(matrix, camera.yaw);
    cml::matrix_set_translation(matrix, camera.position);
    return matrix;
}

} // namespace kloss
