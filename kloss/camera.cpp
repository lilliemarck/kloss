#include "camera.hpp"

#include <boost/algorithm/clamp.hpp>

namespace kloss {

camera::camera()
    : position_(0.0f, 0.0f, 0.0f)
    , yaw_(0.0f)
    , pitch_(0.0f)
{
}

cml::vector3f camera::get_position() const
{
    return position_;
}

void camera::set_position(const cml::vector3f& units)
{
    position_ = units;
}

float camera::get_yaw() const
{
    return yaw_;
}

void camera::set_yaw(float radians)
{
    yaw_ = std::fmod(radians, cml::constantsf::two_pi());
}

float camera::get_pitch() const
{
    return pitch_;
}

void camera::set_pitch(float radians)
{
    pitch_ = boost::algorithm::clamp(radians, -cml::constantsf::pi_over_2(), cml::constantsf::pi_over_2());
}

void move_forward(camera& camera, float units)
{
    camera.set_position(camera.get_position() + units * look_direction(camera));
}

void move_sideways(camera& camera, float units)
{
    camera.set_position(camera.get_position() + units * matrix_get_x_basis_vector(rotation_matrix(camera)));
}

void rotate_yaw(camera& camera, float radians)
{
    camera.set_yaw(camera.get_yaw() + radians);
}

void rotate_pitch(camera& camera, float radians)
{
    camera.set_pitch(camera.get_pitch() + radians);
}

cml::vector3f look_direction(camera const& camera)
{
    return -matrix_get_z_basis_vector(rotation_matrix(camera));
}

cml::matrix44f_c rotation_matrix(camera const& camera)
{
    cml::matrix44f_c matrix;
    matrix_rotation_world_x(matrix, camera.get_pitch());
    matrix_rotate_about_world_y(matrix, camera.get_yaw());
    return matrix;
}

cml::matrix44f_c world_transform(camera const& camera)
{
    cml::matrix44f_c matrix = rotation_matrix(camera);
    matrix_set_translation(matrix, camera.get_position());
    return matrix;
}

ray to_ray(camera const& camera)
{
    return {camera.get_position(), look_direction(camera)};
}

} // namespace kloss
