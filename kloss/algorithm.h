#pragma once

float clampf(float x, float min, float max);
float maxf(float x, float y);
float minf(float x, float y);
int mini(int x, int y);
float roundtofractf(float x, float increment);
float squaref(float x);

/**
 * Searches for occurences of a pointer in a memory range. Returns a pointer to
 * the first occurance of the pointer in the memory range or NULL if not found.
 */
void *find_ptr(void *begin, void *end, void *element);
