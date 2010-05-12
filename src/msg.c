#include "utils.h"
#include "msg.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * All messages available in the system, organised on a freelist.
 */
static msg_t
g_msgs[NUM_MESSAGES];

/*
 * Freelist pointer. Points to the first free message.
 */
static msg_t *
g_freelist;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the freelist like a normal linked list.
 */
static void
msg_init_freelist(void)
{
    size_t i = 0;

    g_freelist = &g_msgs[0];

    for (i = 0; i < COUNT_ARRAY(g_msgs)-1; ++i)
    {
        g_msgs[i].next_free = &g_msgs[i+1];
    }

    g_msgs[COUNT_ARRAY(g_msgs)-1].next_free = NULL;
}

/*
 * XXXXXXXXXReturns a free list node, or NULL if none free.
 */
msg_t *
msg_alloc(void)
{
    static int is_initialized = 0;
    msg_t *msg = NULL;

    /* Make sure the free list is initialized automatically! */
    if (!is_initialized)
    {
        is_initialized = 1;
        msg_init_freelist();
    }

    msg = g_freelist;
    if (!msg)
    {
        return NULL;
    }
    else
    {
        g_freelist = g_freelist->next_free;
        return ZERO_STRUCT(msg);
    }
}

/*
 * XXXXXXXXXXXXReleases a list node back to the system. Always returns NULL, to make it
 * easy and idiomatic to avoid dangling pointers:
 *
 *   msg = msg_alloc();
 *   ... use node ...
 *   msg = msg_free();
 */
msg_t *
msg_free(msg_t *msg)
{
    kdebug_assert(msg);

    msg->next_free = g_freelist;
    g_freelist = msg;
    return NULL;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    XXXXXXXgcc -DUNITTEST -DPCB_MAIN src/pcb.c \
    src/utils.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef MSG_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
   msg_t msg;
   msg.type = MSG_TYPE_ARGUMENT;

   msg.data_type = MSG_DATA_TYPE_INTEGER;
   msg.data.integer = 666;
   if (msg.data_type == MSG_DATA_TYPE_INTEGER)
   {
       printf("data.integer = %d\n", msg.data.integer);
   }

   msg.data_type = MSG_DATA_TYPE_STRING;
   msg.data.string[0] = 'T';
   msg.data.string[1] = 'j';
   msg.data.string[2] = 'o';
   msg.data.string[3] = '!';
   msg.data.string[4] = 0;
   if (msg.data_type == MSG_DATA_TYPE_STRING)
   {
       printf("data.string = %s\n", msg.data.string);
   }

   return 0;
}

#endif
