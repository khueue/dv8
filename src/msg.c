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
    static int is_initialized = 0;
    if (!is_initialized)
    {
        is_initialized = 1;
        size_t i = 0;

        g_freelist = &g_msgs[0];

        for (i = 0; i < COUNT_ARRAY(g_msgs)-1; ++i)
        {
            g_msgs[i].next_free = &g_msgs[i+1];
        }

        g_msgs[COUNT_ARRAY(g_msgs)-1].next_free = NULL;
    }
}

/*
 * Returns the number of free messages in the system.
 */
size_t
msg_num_free(void)
{
    const msg_t *msg = NULL;
    size_t num = 0;

    msg_init_freelist();

    msg = g_freelist;
    while (msg)
    {
        ++num;
        msg = msg->next_free;
    }

    return num;
}

msg_t *
msg_alloc(void)
{
    msg_t *msg = NULL;

    msg_init_freelist();

    msg = g_freelist;
    if (!msg)
    {
        return NULL;
    }
    else
    {
        g_freelist = g_freelist->next_free;
        return msg_zero(msg);
    }
}

msg_t *
msg_free(msg_t *msg)
{
    kdebug_assert(msg);

    msg->next_free = g_freelist;
    g_freelist = msg;
    return NULL;
}

/*
 * XXXXXX
 */
void
msg_copy(msg_t *dst, const msg_t *src)
{
    memcpy(dst, src, sizeof(*dst));
}

msg_t *
msg_zero(msg_t *msg)
{
    ZERO_STRUCT(msg);
    msg_type_set_invalid(msg);
    return msg;
}

msg_type_t
msg_get_type(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->type;
}

int
msg_cmp_priority(const void *pmsg1, const void *pmsg2)
{
    const msg_t *msg1 = (const msg_t *)pmsg1;
    const msg_t *msg2 = (const msg_t *)pmsg2;

    kdebug_assert(msg1);
    kdebug_assert(msg2);

    return msg1->priority - msg2->priority;
}

int
msg_has_type(const void *pmsg, const void *ptype)
{
    const msg_t *msg = NULL;
    const msg_type_t *type = NULL;

    kdebug_assert(pmsg);
    kdebug_assert(ptype);

    msg  = (const msg_t *)pmsg;
    type = (const msg_type_t *)ptype;

    return msg_type_is(msg, *type);
}

int
msg_type_is(const msg_t *msg, msg_type_t type)
{
    kdebug_assert(msg);

    return msg->type == type;
}

void
msg_set_type(msg_t *msg, msg_type_t type)
{
    kdebug_assert(msg);

    msg->type = type;
}

int
msg_type_is_invalid(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->type == MSG_TYPE_INVALID;
}

void
msg_type_set_invalid(msg_t *msg)
{
    kdebug_assert(msg);

    msg->type = MSG_TYPE_INVALID;
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

void
msg_set_receiver_pid(msg_t *msg, uint32_t receiver_pid)
{
    kdebug_assert(msg);

    msg->receiver_pid = receiver_pid;
}

uint32_t
msg_get_receiver_pid(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->receiver_pid;
}

int
msg_data_is_invalid(const msg_t *msg)
{
    kdebug_assert(msg);

    return msg->data_type == MSG_DATA_TYPE_INVALID;
}

void
msg_data_set_invalid(msg_t *msg)
{
    kdebug_assert(msg);

    msg->data_type = MSG_DATA_TYPE_INVALID;
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

    return msg_data_is_integer(msg) ? msg->data.integer : 0;
}

msg_t *
create_string_message(const char *str, uint32_t receiver_pid)
{
    msg_t *msg = NULL;

    kdebug_assert(str);

    msg = msg_alloc();
    if (!msg)
    {
        /* Error handling? XXX */
        return NULL;
    }

    msg_data_set_string(msg, str);
    msg_set_receiver_pid(msg, receiver_pid);

    return msg;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DMSG_MAIN src/msg.c \
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
