#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "debug.h"
#include "bits.h"

/*
 * Hack for easy unit testing. This lets us use kdebug_assert() without
 * compiling debug.c (which requires tty etc.).
 */
#ifdef UNITTEST
    #include <assert.h>
    #undef kdebug_assert
    #define kdebug_assert(condition) assert(condition)
#endif

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
#define ZERO_ARRAY(arr)  (  MEMZERO((arr), sizeof( (arr)))  )
#define ZERO_STRUCT(p)   (  MEMZERO((p),   sizeof(*(p)))    )

/*
 * Fills array/struct with given uint8_t value.
 */
#define FILL_ARRAY(arr, value)  (  memset((arr), (value), sizeof(arr))   )
#define FILL_STRUCT(p, value)   (  memset((p),   (value), sizeof(*(p)))  )

/*
 * Fills an object pointed to by p with zeroes. Called indirectly by the
 * macros ZERO_STRUCT/ARRAY above.
 */
#define MEMZERO(p, size)  (  memset((p), 0, (size))  )

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

size_t
strlen(const char *s);

char *
strcpy(char *dst, const char *src);

int
strcmp(const char *a, const char *b);

void *
memset(void *dst, int value, size_t num_bytes);

void *
memcpy(void *dst, const void *src, size_t num_bytes);

#endif
