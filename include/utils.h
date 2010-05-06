#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "debug.h"
#include "bits.h"

/*
 * ---------------------------------------------------------------------------
 * Macros.
 * ---------------------------------------------------------------------------
 */

/*
 * Returns the maximum of two values.
 */
#define MAX(a, b)  (  ((a) > (b)) ? (a) : (b)  )

/*
 * Counts the number of elements in an array. Note that this only works with
 * an actual array, and not a pointer which used to be an array (like an
 * array passed to a function)!
 */
#define COUNT_ARRAY(arr)  (  sizeof(arr) / sizeof((arr)[0])  )

/*
 * Fills array/struct with zeroes.
 */
#define ZERO_ARRAY(p)   (  zero_fill((p), sizeof( (p)))  )
#define ZERO_STRUCT(p)  (  zero_fill((p), sizeof(*(p)))  )

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void *
zero_fill(void *p, size_t num_bytes);

#endif
