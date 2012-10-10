#include "camera.hpp"
#include <boost/algorithm/clamp.hpp>
#include <kloss/algorithm.hpp>
#include <kloss/geometry.hpp>
#include <kloss/math.hpp>

camera::camera()
    : position_()
    , yaw_()
    , pitch_()
{
}

vec3 camera::get_position() const noexcept
{
    return position_;
}

void camera::set_position(vec3 const& units) noexcept
{
    position_ = units;
}

float camera::get_yaw() const noexcept
{
    return yaw_;
}

void camera::set_yaw(float radians) noexcept
{
    yaw_ = fmod(radians, tau);
}

float camera::get_pitch() const noexcept
{
    return pitch_;
}

void camera::set_pitch(float radians) noexcept
{
    pitch_ = boost::algorithm::clamp(radians, -tau_over_4, tau_over_4);
}

void move_forward(camera& camera, float units)
{
    camera.set_position(camera.get_position() + look_direction(camera) * units);
}

void move_sideways(camera& camera, float units)
{
    mat4 rotation = rotation_matrix(camera);
    vec3 right = xyz(rotation.x);
    camera.set_position(camera.get_position() + right * units);
}

void rotate_yaw(camera& camera, float radians)
{
    camera.set_yaw(camera.get_yaw() + radians);
}

void rotate_pitch(camera& camera, float radians)
{
    camera.set_pitch(camera.get_pitch() + radians);
}

vec3 look_direction(camera const& camera)
{
    return xyz(rotation_matrix(camera).y);
}

mat4 rotation_matrix(camera const& camera)
{
    return rotation_matrix_z(camera.get_yaw()) * rotation_matrix_x(camera.get_pitch());
}

mat4 world_transform(camera const& camera)
{
    mat4 transform = rotation_matrix(camera);
    vec3 pos = camera.get_position();

    transform.t.x = pos.x;
    transform.t.y = pos.y;
    transform.t.z = pos.z;

    return transform;
}

ray camera_ray(camera const& camera)
{
    return {camera.get_position(), look_direction(camera)};
}
