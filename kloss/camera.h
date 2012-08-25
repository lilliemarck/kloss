#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct Mat4;
struct Vec3;
struct Ray;

typedef struct Camera Camera;

Camera *CreateCamera(void);
void DestroyCamera(Camera *camera);
void GetCameraPosition(Camera const *camera, struct Vec3 *out);
void SetCameraPosition(Camera *camera, struct Vec3 *out);
float GetCameraYaw(Camera const *camera);
void SetCameraYaw(Camera *camera, float radians);
float GetCameraPitch(Camera const *camera);
void SetCameraPitch(Camera *camera, float radians);

void MoveCameraForward(Camera *camera, float units);
void MoveCameraSideways(Camera *camera, float units);
void RotateCameraYaw(Camera *camera, float radians);
void RotateCameraPitch(Camera *camera, float radians);
void CameraLookDirection(Camera const *camera, struct Vec3 *out);
void CameraRotationMatrix(Camera const *camera, struct Mat4 *out);
void CameraWorldTransform(Camera const *camera, struct Mat4 *out);
/**
 * Create a ray from the camera's position and look direction.
 */
void CameraRay(Camera const *camera, struct Ray *out);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
