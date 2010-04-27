#include <stdio.h>

#include "utils.h"

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
