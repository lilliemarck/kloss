#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

float clampf(float x, float min, float max);
float maxf(float x, float y);
float minf(float x, float y);
int mini(int x, int y);
float roundtofractf(float x, float increment);
float squaref(float x);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
