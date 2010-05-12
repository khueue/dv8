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

int
msg_type_is_unknown(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->type == MSG_TYPE_UNKNOWN;
}

void
msg_type_set_unknown(msg_t *msg)
{
    kdebug_assert(msg);

    msg->type = MSG_TYPE_UNKNOWN;
}

int
msg_type_is_argument(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->type == MSG_TYPE_ARGUMENT;
}

void
msg_type_set_argument(msg_t *msg)
{
    kdebug_assert(msg);

    msg->type = MSG_TYPE_ARGUMENT;
}

int
msg_type_is_console_input(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->type == MSG_TYPE_CONSOLE_INPUT;
}

void
msg_type_set_console_input(msg_t *msg)
{
    kdebug_assert(msg);

    msg->type = MSG_TYPE_CONSOLE_INPUT;
}

int
msg_data_is_unknown(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->data_type == MSG_DATA_TYPE_UNKNOWN;
}

void
msg_data_set_unknown(msg_t *msg)
{
    kdebug_assert(msg);

    msg->data_type = MSG_DATA_TYPE_UNKNOWN;
}

void
msg_set_priority(msg_t *msg, uint32_t priority)
{
    kdebug_assert(msg);

    msg->priority = priority;
}

uint32_t
msg_get_priority(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->priority;
}

void
msg_set_sender_pid(msg_t *msg, uint32_t sender_pid)
{
    kdebug_assert(msg);

    msg->sender_pid = sender_pid;
}

uint32_t
msg_get_sender_pid(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->sender_pid;
}

int
msg_data_is_string(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->data_type == MSG_DATA_TYPE_STRING;
}

/*
 * Makes a _copy_ of the string.
 */
void
msg_data_set_string(msg_t *msg, const char string[])
{
    kdebug_assert(msg);
    kdebug_assert(string);

    msg->data_type = MSG_DATA_TYPE_STRING;
    strcpy(msg->data.string, string);
    msg->data.string[sizeof(msg->data.string)-1] = '\0';
}

const char *
msg_data_get_string(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg_data_is_string(msg) ? msg->data.string : NULL;
}

int
msg_data_is_integer(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->data_type == MSG_DATA_TYPE_INTEGER;
}

void
msg_data_set_integer(msg_t *msg, int integer)
{
    kdebug_assert(msg);

    msg->data_type = MSG_DATA_TYPE_INTEGER;
    msg->data.integer = integer;
}

int
msg_data_get_integer(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg_data_is_integer(msg) ? msg->data.integer : 0; /* XXXX 0? */
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
