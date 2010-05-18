#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * See string.h documentation.
 */
void *
memcpy(void *dst, const void *src, size_t num_bytes)
{
    uint8_t       *dst_byte = dst;
    const uint8_t *src_byte = src;
    size_t i = 0;

    for (i = 0; i < num_bytes; ++i)
    {
        *dst_byte++ = *src_byte++;
    }

    return dst;
}

/*
 * See string.h documentation.
 */
void *
memset(void *dst, int value, size_t num_bytes)
{
    uint8_t *dst_byte = dst;
    uint8_t byte_value = (uint8_t)value;
    size_t i = 0;

    for (i = 0; i < num_bytes; ++i)
    {
        dst_byte[i] = byte_value;
    }

    return dst;
}

/*
 * See string.h documentation.
 */
size_t
strlen(const char *s)
{
    size_t len = 0;

    while (*s++)
    {
        ++len;
    }

    return len;
}

/*
 * See string.h documentation.
 */
int
strcmp(const char *a, const char *b)
{
    while (*a && *b && (*a == *b))
    {
        ++a;
        ++b;
    }

    return *a - *b;
}

/*
 * See string.h documentation.
 */
char *
strcpy(char *dst, const char *src)
{
    char *dst_orig = dst;

    while (*src)
    {
        *dst++ = *src++;
    }
    *dst = '\0';

    return dst_orig;
}

/*
 * See stdlib.h documentation.
 */
int
atoi(const char *str)
{
    int x = 0;
    int i = 0;
    int j = 1;
    int k = 0;

    kdebug_assert(str);

    /* Skip white space */
    while (str[i] == ' ')
    {
        i++;
    }

    if (str[i] == '-' || str[i] == '+')
    {
        i++;
    }

    /* index of start */
    k = i;

    /* 0-9 = 48-57 */
    while (str[i] >= '0' && str[i] <= '9')
    {
        i++;
    }

    /* remove last char \0 */
    i--;

    for ( ; i >= k; i--)
    {
        x += (str[i] - '0') * j;
        j *= 10;
    }

    if (str[i] == '-')
    {
        x = -x;
    }

    return x;
}

/*
 * See string.h documentation
 */
char *
strchr(char *str, int c)
{
    while (*str && *str != c)
    {
        ++str;
    }
    return (*str == c) ? str : NULL;
}

/*
 * Seed used for random number generation.
 */
static unsigned long
g_next_rand = 1;

/*
 * Seed the random number generator.
 */
void
srand(uint32_t seed)
{
    g_next_rand = seed;
}

#define RAND_MAX 32767

/*
 * Returns a pseudo-random number in [0, RAND_MAX).
 */
int
rand(void)
{
    g_next_rand = g_next_rand*1103515245 + 12345;
    return (int)(g_next_rand / 65536) % (RAND_MAX + 1);
}

/*
 * Returns a pseudo-random number in [min, max].
 */
int
rand_between(int min, int max)
{
    int r = rand();

    kdebug_assert(min >= 0);
    kdebug_assert(max >= 0);

    while (r < min || r > max)
    {
        r = rand();
    }
    return r;
}


int printf ( const char * format, ... );

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DUTILS_MAIN src/utils.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef UTILS_MAIN

#include <stdio.h>

struct stuff
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
        printf("\n");
    }

    /* Zero a struct. */
    {
        struct stuff st;
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
        printf("\n");
    }

    /* Count array length. */
    {
        double d[3];
        printf("length  = %d\n", COUNT_ARRAY(d));
        printf("\n");
    }

    /* Test maximum. */
    {
        int a = 4;
        int b = 5;
        printf("max     = %d\n", MAX(a, b));
        printf("max     = %d\n", MAX(b, a));
        printf("\n");
    }

    /* Test strcmp. */
    {
        char a[] = "ab";
        char b[] = "abd";
        printf("strcmp  = %d\n", strcmp(a, b));
        printf("\n");
    }

    /* Test strlen. */
    {
        char a[] = "ab\n";
        char b[] = "";
        printf("strlen  = %d\n", strlen(a)); /* = 3 */
        printf("strlen  = %d\n", strlen(b)); /* = 0 */
        printf("\n");
    }

    /* Test strcpy. */
    {
        char dst[20] = "";
        char src[20] = "baba";
        printf("strcpy  = %s\n", strcpy(dst, src));
        printf("\n");
    }

    /* Test memset. */
    {
        char dst[20] = "";
        printf("memset  = %s\n", (char *)memset(dst, 97, sizeof(dst)-1));
        printf("\n");
    }

    /* Test FILL_ARRAY. */
    {
        int arr[2];
        size_t i = 0;
        uint8_t *byte = (uint8_t *)arr;
        FILL_ARRAY(arr, 0xba);
        printf("arr[0]  = %d\n", arr[0]);
        printf("arr[1]  = %d\n", arr[1]);
        for (i = 0; i < sizeof(arr); ++i)
        {
            printf("arr as byte[%d]  = 0x%x\n", i, *byte++);
        }
        printf("\n");
    }

    /* Test atoi. */
    {
        char str[] = "0123456789";
        char str1[] = "-489";
        char str2[] = "  9 white space";
        char str3[] = "--5";
        char str4[] = "";
        printf("%s = %d\n", str, atoi(str));
        printf("%s = %d\n", str1, atoi(str1));
        printf("%s = %d\n", str2, atoi(str2));
        printf("%s = %d\n", str3, atoi(str3));
        printf("%s = %d\n", str4, atoi(str4));
        printf("\n");
    }

    /* Test strchar. */
    {
        char str[] = "0123456789";
        printf("%s\n", strchr(str,'5'));
        printf("%s\n", strchr(str,'1'));
        printf("\n");
    }

    /* Test rand. */
    {
        size_t i = 0;
        for (i = 0; i < 5; ++i)
        {
            printf("rand() = %d\n", rand());
        }
        for (i = 0; i < 10; ++i)
        {
            printf("rand_between(1, 3) = %d\n", rand_between(1, 3));
        }
        while (rand() != 0)
        {
        }
        while (rand() != RAND_MAX)
        {
        }
        printf("\n");
    }

    return 0;
}

#endif
