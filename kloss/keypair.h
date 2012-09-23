#pragma once

#include <stdbool.h>

/**
 * Convert two mutually exclusive inputs to a single output value.
 */
typedef struct keypair keypair;

keypair *create_keypair(void);
void destroy_keypair(keypair *pair);

/**
 * Update the key state of the first key and return the new value.
 */
float set_keypair_first(keypair *pair, bool pressed);

/**
 * Update the key state of the second key and return the new value.
 */
float set_keypair_second(keypair *pair, bool pressed);

/**
 * Returns the value which is -1 if the first key is selected, 1 if the second
 * key is selected or 0 if none of the keys are pressed.
 */
float keypair_value(keypair const *pair);
