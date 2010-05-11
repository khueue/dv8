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
};

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
