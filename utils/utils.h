typedef unsigned char uint8_t;   /* include "types.h" instead */

/*
 * ---------------------------------------------------------------------------
 * Macros.
 * ---------------------------------------------------------------------------
 */

/*
 * Counts the number of elements in an array. Note that this only works with
 * an actual array, and not a pointer which used to be an array (like an
 * array passed to a function)!
 */
#define COUNT_ARRAY(arr)  ( sizeof(arr) / sizeof((arr)[0]) )

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

/*
 * Fills an object at p with num_bytes zeroes. Use the provided macros above
 * instead of using this function directly.
 */
void
zero_fill(void *p, size_t num_bytes)
{
    uint8_t *byte = p;
    size_t i;

    for (i = 0; i < num_bytes; ++i)
    {
        byte[i] = 0;
    }
}
