#include "camera.hpp"

#include <boost/algorithm/clamp.hpp>

namespace kloss {

camera::camera()
    : position_{0.0f, 0.0f, 0.0f}
    , yaw_{0.0f}
    , pitch_{0.0f}
{
}

Vec3 camera::get_position() const
{
    return position_;
}

void camera::set_position(const Vec3& units)
{
    position_ = units;
}

float camera::get_yaw() const
{
    return yaw_;
}

void camera::set_yaw(float radians)
{
    yaw_ = std::fmod(radians, float(M_TAU));
}

float camera::get_pitch() const
{
    return pitch_;
}

void camera::set_pitch(float radians)
{
    pitch_ = boost::algorithm::clamp(radians, -M_TAU_4, M_TAU_4);
}

void move_forward(camera& camera, float units)
{
    Vec3 pos = camera.get_position();
    Vec3 dir = look_direction(camera);
    Vec3AddScaled(&pos, &pos, &dir, units);
    camera.set_position(pos);
}

void move_sideways(camera& camera, float units)
{
    Mat4 mat = rotation_matrix(camera);
    Vec3 pos = camera.get_position();
    Vec3 right = {mat.X.X, mat.X.Y, mat.X.Z};
    Vec3AddScaled(&pos, &pos, &right, units);
    camera.set_position(pos);
}

void rotate_yaw(camera& camera, float radians)
{
    camera.set_yaw(camera.get_yaw() + radians);
}

void rotate_pitch(camera& camera, float radians)
{
    camera.set_pitch(camera.get_pitch() + radians);
}

Vec3 look_direction(camera const& camera)
{
    Mat4 matrix = rotation_matrix(camera);
    return {matrix.Y.X, matrix.Y.Y, matrix.Y.Z};
}

Mat4 rotation_matrix(camera const& camera)
{
    Mat4 rotx, rotz, out;

    Mat4RotationX(&rotx, camera.get_pitch());
    Mat4RotationZ(&rotz, camera.get_yaw());
    Mat4Transform(&out, &rotx, &rotz);

    return out;
}

Mat4 world_transform(camera const& camera)
{
    Mat4 matrix = rotation_matrix(camera);
    Vec3 pos    = camera.get_position();
    matrix.T.X = pos.X;
    matrix.T.Y = pos.Y;
    matrix.T.Z = pos.Z;
    return matrix;
}

ray to_ray(camera const& camera)
{
    return {camera.get_position(), look_direction(camera)};
}

} // namespace kloss
