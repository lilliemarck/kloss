#pragma once

struct mat4;
struct vec3;
struct ray;

typedef struct camera camera;

camera *create_camera(void);
void destroy_camera(camera *camera);
void get_camera_position(camera const *camera, struct vec3 *out);
void set_camera_position(camera *camera, struct vec3 *out);
float get_camera_yaw(camera const *camera);
void set_camera_yaw(camera *camera, float radians);
float get_camera_pitch(camera const *camera);
void set_camera_pitch(camera *camera, float radians);

void move_camera_forward(camera *camera, float units);
void move_camera_sideways(camera *camera, float units);
void rotate_camera_yaw(camera *camera, float radians);
void rotate_camera_pitch(camera *camera, float radians);
void camera_lookdirection(camera const *camera, struct vec3 *out);
void camera_rotationmatrix(camera const *camera, struct mat4 *out);
void camera_worldtransform(camera const *camera, struct mat4 *out);
/**
 * Create a ray from the camera's position and look direction.
 */
void camera_ray(camera const *camera, struct ray *out);
