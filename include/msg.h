#ifndef MSG_H
#define MSG_H

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Determines the type of the message.
 */
typedef enum
{
    MSG_TYPE_UNKNOWN,
    MSG_TYPE_ARGUMENT,
    MSG_TYPE_CONSOLE_INPUT
} msg_type_t;

/*
 * Determines the type of data the message contains.
 */
typedef enum
{
    MSG_DATA_TYPE_UNKNOWN,
    MSG_DATA_TYPE_INTEGER,
    MSG_DATA_TYPE_STRING
} msg_data_type_t;

/*
 * Message used for inter-process communication.
 */
typedef struct _msg msg_t;
struct _msg
{
    msg_type_t type;
    msg_data_type_t data_type;
    union
    {
        int integer;
        char string[1024];
    } data;

    uint32_t sender_pid;
    int priority;

    /* Internal freelist pointer. */
    msg_t *next_free;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

int
msg_is_unknown(const msg_t *msg);

int
msg_is_argument(const msg_t *msg);

int
msg_is_console_input(const msg_t *msg);

int
msg_contains_unknown(const msg_t *msg);

int
msg_contains_string(const msg_t *msg);

int
msg_contains_integer(const msg_t *msg);

msg_t *
msg_alloc(void);

msg_t *
msg_free(msg_t *msg);

#endif
