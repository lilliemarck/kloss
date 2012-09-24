#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

float Clampf(float x, float min, float max);
float Maxf(float x, float y);
float Minf(float x, float y);
int Mini(int x, int y);
float RoundToStepf(float x, float increment);
float Squaref(float x);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
