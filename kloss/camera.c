#include "camera.h"
#include <kloss/algorithm.h>
#include <kloss/geometry.h>
#include <kloss/math.h>
#include <math.h>
#include <stdlib.h>

struct Camera
{
    Vec3 Position;
    float Yaw;
    float Pitch;
};

Camera *CreateCamera(void)
{
    return calloc(1, sizeof(Camera));
}

void DestroyCamera(Camera *camera)
{
    free(camera);
}

void GetCameraPosition(Camera const *camera, Vec3 *out)
{
    *out = camera->Position;
}

void SetCameraPosition(Camera *camera, Vec3 *out)
{
    camera->Position = *out;
}

float GetCameraYaw(Camera const *camera)
{
    return camera->Yaw;
}

void SetCameraYaw(Camera *camera, float radians)
{
    camera->Yaw = fmod(radians, M_TAU);
}

float GetCameraPitch(Camera const *camera)
{
    return camera->Pitch;
}

void SetCameraPitch(Camera *camera, float radians)
{
    camera->Pitch = Clampf(radians, -M_TAU_4, M_TAU_4);
}

void MoveCameraForward(Camera *camera, float units)
{
    Vec3 direction;

    CameraLookDirection(camera, &direction);
    Vec3AddScaled(&camera->Position, &camera->Position, &direction, units);
}

void MoveCameraSideways(Camera *camera, float units)
{
    Mat4 rotation;
    Vec3 right;

    CameraRotationMatrix(camera, &rotation);
    Vec4XYZ(&right, &rotation.X);
    Vec3AddScaled(&camera->Position, &camera->Position, &right, units);
}

void RotateCameraYaw(Camera *camera, float radians)
{
    SetCameraYaw(camera, camera->Yaw + radians);
}

void RotateCameraPitch(Camera *camera, float radians)
{
    SetCameraPitch(camera, camera->Pitch + radians);
}

void CameraLookDirection(Camera const *camera, Vec3 *out)
{
    Mat4 rotation;

    CameraRotationMatrix(camera, &rotation);
    Vec4XYZ(out, &rotation.Y);
}

void CameraRotationMatrix(Camera const *camera, Mat4 *out)
{
    Mat4 rotx, rotz;

    Mat4RotationX(&rotx, camera->Pitch);
    Mat4RotationZ(&rotz, camera->Yaw);
    Mat4Transform(out, &rotx, &rotz);
}

void CameraWorldTransform(Camera const *camera, Mat4 *out)
{
    CameraRotationMatrix(camera, out);
    out->T.X = camera->Position.X;
    out->T.Y = camera->Position.Y;
    out->T.Z = camera->Position.Z;
}

void CameraRay(Camera const *camera, Ray *out)
{
    out->Origin = camera->Position;
    CameraLookDirection(camera, &out->Direction);
}
