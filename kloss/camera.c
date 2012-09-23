#include "camera.h"
#include <kloss/algorithm.h>
#include <kloss/geometry.h>
#include <kloss/math.h>
#include <math.h>
#include <stdlib.h>

struct camera
{
    vec3 position;
    float yaw;
    float pitch;
};

camera *create_camera(void)
{
    return calloc(1, sizeof(camera));
}

void destroy_camera(camera *camera)
{
    free(camera);
}

void get_camera_position(camera const *camera, vec3 *out)
{
    *out = camera->position;
}

void set_camera_position(camera *camera, vec3 *out)
{
    camera->position = *out;
}

float get_camera_yaw(camera const *camera)
{
    return camera->yaw;
}

void set_camera_yaw(camera *camera, float radians)
{
    camera->yaw = fmod(radians, M_TAU);
}

float get_camera_pitch(camera const *camera)
{
    return camera->pitch;
}

void set_camera_pitch(camera *camera, float radians)
{
    camera->pitch = clampf(radians, -M_TAU_4, M_TAU_4);
}

void move_camera_forward(camera *camera, float units)
{
    vec3 direction;

    camera_lookdirection(camera, &direction);
    vec3_add_scaled(&camera->position, &camera->position, &direction, units);
}

void move_camera_sideways(camera *camera, float units)
{
    mat4 rotation;
    vec3 right;

    camera_rotationmatrix(camera, &rotation);
    vec4_xyz(&right, &rotation.x);
    vec3_add_scaled(&camera->position, &camera->position, &right, units);
}

void rotate_camera_yaw(camera *camera, float radians)
{
    set_camera_yaw(camera, camera->yaw + radians);
}

void rotate_camera_pitch(camera *camera, float radians)
{
    set_camera_pitch(camera, camera->pitch + radians);
}

void camera_lookdirection(camera const *camera, vec3 *out)
{
    mat4 rotation;

    camera_rotationmatrix(camera, &rotation);
    vec4_xyz(out, &rotation.y);
}

void camera_rotationmatrix(camera const *camera, mat4 *out)
{
    mat4 rotx, rotz;

    mat4_rotationx(&rotx, camera->pitch);
    mat4_rotationz(&rotz, camera->yaw);
    mat4_transform(out, &rotx, &rotz);
}

void camera_worldtransform(camera const *camera, mat4 *out)
{
    camera_rotationmatrix(camera, out);
    out->t.x = camera->position.x;
    out->t.y = camera->position.y;
    out->t.z = camera->position.z;
}

void camera_ray(camera const *camera, ray *out)
{
    out->origin = camera->position;
    camera_lookdirection(camera, &out->direction);
}
