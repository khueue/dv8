#ifndef MSG_H
#define MSG_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Message used for inter-process communication.
 */
typedef struct msg_ msg_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Returns a free message, or NULL if none free.
 */
msg_t *
msg_alloc(void);

/*
 * Releases a message back to the system. Always returns NULL, to make it easy
 * to avoid dangling pointers.
 */
msg_t *
msg_free(msg_t *msg);

/*
 * - is() returns true if the message is of an unknown type
 * - set() makes the message be of an unknown type
 */
int  msg_type_is_unknown(const msg_t *msg);
void msg_type_set_unknown(msg_t *msg);

/*
 * - is() returns true if the message represents an argument
 * - set() makes the message represent an argument
 */
int  msg_type_is_argument(const msg_t *msg);
void msg_type_set_argument(msg_t *msg);

/*
 * - is() returns true if the message represents console input
 * - set() makes the message represent console input
 */
int  msg_type_is_console_input(const msg_t *msg);
void msg_type_set_console_input(msg_t *msg);

/*
 * - set() sets the priority of the message
 * - get() returns the priority of the message
 */
void     msg_set_priority(msg_t *msg, uint32_t priority);
uint32_t msg_get_priority(const msg_t *msg);

/*
 * - set() sets the sender's pid of the message
 * - get() returns the sender's pid of the message
 */
void     msg_set_sender_pid(msg_t *msg, uint32_t sender_pid);
uint32_t msg_get_sender_pid(const msg_t *msg);

/*
 * - set() sets the receiver's pid of the message
 * - get() returns the receiver's pid of the message
 */
void     msg_set_receiver_pid(msg_t *msg, uint32_t receiver_pid);
uint32_t msg_get_receiver_pid(const msg_t *msg);

/*
 * - is() returns true if the data within the message is unknown
 * - set() makes the data within the message be unknown
 */
int  msg_data_is_unknown(const msg_t *msg);
void msg_data_set_unknown(msg_t *msg);

/*
 * - is() returns true if the data within the message is string
 * - set() copies the given string into the message data and sets the
 *     message's data type to string
 * - get() returns the string data of the message, or NULL if the data type is
 *     not string
 */
int         msg_data_is_string(const msg_t *msg);
void        msg_data_set_string(msg_t *msg, const char string[]);
const char *msg_data_get_string(const msg_t *msg);

/*
 * - is() returns true if the data within the message is integer
 * - set() copies the given integer into the message data and sets the
 *     message's data type to integer
 * - get() returns the integer data of the message, or 0 if the data type is
 *     not integer
 */
int  msg_data_is_integer(const msg_t *msg);
void msg_data_set_integer(msg_t *msg, int integer);
int  msg_data_get_integer(const msg_t *msg);

/*
 * Allocates and initializes a message with a string.
 */
msg_t *
create_string_message(const char *str, uint32_t receiver_pid);

#endif
