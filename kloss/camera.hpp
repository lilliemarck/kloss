#pragma once

#include <kloss/math.hpp>

struct ray;

class camera
{
public:
    camera();

    vec3 get_position() const noexcept;
    void set_position(vec3 const& units) noexcept;
    float get_yaw() const noexcept;
    void set_yaw(float radians) noexcept;
    float get_pitch() const noexcept;
    void set_pitch(float radians) noexcept;

private:
    vec3 position_;
    float yaw_;
    float pitch_;
};

void move_forward(camera& camera, float units);
void move_sideways(camera& camera, float units);
void rotate_yaw(camera& camera, float radians);
void rotate_pitch(camera& camera, float radians);
vec3 look_direction(camera const& camera);
mat4 rotation_matrix(camera const& camera);
mat4 world_transform(camera const& camera);
/**
 * Create a ray from the camera's position and look direction.
 */
ray camera_ray(camera const& camera);
