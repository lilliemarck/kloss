#pragma once

#include <cml/cml.h>

namespace kloss {

class camera
{
public:
    camera();

    cml::vector3f get_position() const;
    void set_position(cml::vector3f const& units);

    float get_yaw() const;
    void set_yaw(float radians);

    float get_pitch() const;
    void set_pitch(float radians);

private:
    cml::vector3f position_;
    float yaw_;
    float pitch_;
};

void move_forward(camera& camera, float units);
void move_sideways(camera& camera, float units);
void rotate_yaw(camera& camera, float radians);
void rotate_pitch(camera& camera, float radians);
cml::matrix44f_c rotation_matrix(camera const& camera);
cml::matrix44f_c world_transform(camera const& camera);

} // namespace kloss
