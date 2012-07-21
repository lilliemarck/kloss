#include "camera.hpp"

namespace kloss {

camera::camera()
    : position(0.0f, 0.0f, 0.0f)
    , yaw(0.0f)
    , pitch(0.0f)
{
}

cml::vector3f forward_vector(camera const& camera)
{
    return -matrix_get_z_basis_vector(rotation_matrix(camera));
}

cml::vector3f right_vector(camera const& camera)
{
    return matrix_get_x_basis_vector(rotation_matrix(camera));
}

cml::matrix44f_c rotation_matrix(camera const& camera)
{
    cml::matrix44f_c matrix;
    matrix_rotation_world_x(matrix, camera.pitch);
    matrix_rotate_about_world_y(matrix, camera.yaw);
    return matrix;
}

cml::matrix44f_c world_transform(camera const& camera)
{
    cml::matrix44f_c matrix = rotation_matrix(camera);
    matrix_set_translation(matrix, camera.position);
    return matrix;
}

} // namespace kloss
