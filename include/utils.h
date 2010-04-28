#ifndef UTILS_H
#define UTILS_H

#include "types.h"

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
 * -------------------------------------------------------------------------
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

#ifdef MAIN

#include <stdio.h>

struct pair
{
    double d[2];
    char str[10];
    int x;
};

int
main(void)
{
    /* Zero an array. */
    {
        double d[2];
        d[0] = 1.0;
        d[1] = 2.0;
        ZERO_ARRAY(d); /* Pass pointer to array. */
        printf("d[0]    = %f\n", d[0]);
        printf("d[1]    = %f\n", d[1]);
    }

    /* Zero a struct. */
    {
        struct pair st;
        st.d[0]   = 1.0;
        st.d[1]   = 2.0;
        st.str[0] = 'O';
        st.str[1] = 'K';
        st.str[2] = '\n';
        st.str[3] = '\0';
        st.x      = -5;
        ZERO_STRUCT(&st); /* Pass pointer to struct. */
        printf("st.d[0] = %f\n",     st.d[0]);
        printf("st.d[1] = %f\n",     st.d[1]);
        printf("st.str  = \"%s\"\n", st.str);
        printf("st.x    = %d\n",     st.x);
    }

    return 0;
}

#endif

#endif
