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
   MSG_TYPE_ARGUMENT,
   MSG_TYPE_OTHER
} msg_type_t;

/*
 * Determines the type of data the message contains.
 */
typedef enum
{
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
   int priority;
   msg_data_type_t data_type;
   union
   {
       int  integer;
       char string[1024];
   } data;
   
   /* Internal freelist pointer. */
   msg_t *next_free;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

msg_t *
msg_alloc(void);

msg_t *
msg_free(msg_t *msg);

#endif
