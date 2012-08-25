#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Convert two mutually exclusive inputs to a single output value.
 */
typedef struct KeyPair KeyPair;

KeyPair *CreateKeyPair(void);
void DestroyKeyPair(KeyPair *pair);

/**
 * Update the key state of the first key and return the new value.
 */
float KeyPairSetFirst(KeyPair *pair, bool pressed);

/**
 * Update the key state of the second key and return the new value.
 */
float KeyPairSetSecond(KeyPair *pair, bool pressed);

/**
 * Returns the value which is -1 if the first key is selected, 1 if the second
 * key is selected or 0 if none of the keys are pressed.
 */
float KeyPairValue(KeyPair const *pair);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
