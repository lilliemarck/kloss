#pragma once

#include <kloss/geometry.hpp>
#include <kloss/math.h>

namespace kloss {

class camera
{
public:
    camera();

    Vec3 get_position() const;
    void set_position(Vec3 const& units);

    float get_yaw() const;
    void set_yaw(float radians);

    float get_pitch() const;
    void set_pitch(float radians);

private:
    Vec3 position_;
    float yaw_;
    float pitch_;
};

void move_forward(camera& camera, float units);
void move_sideways(camera& camera, float units);
void rotate_yaw(camera& camera, float radians);
void rotate_pitch(camera& camera, float radians);

Vec3 look_direction(camera const& camera);
Mat4 rotation_matrix(camera const& camera);
Mat4 world_transform(camera const& camera);

/**
 * Create a ray from the camera's position and look direction.
 */
ray to_ray(camera const& camera);

} // namespace kloss
