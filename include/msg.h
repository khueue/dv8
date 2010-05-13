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
typedef struct _msg msg_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

msg_t *
msg_alloc(void);

msg_t *
msg_free(msg_t *msg);

int  msg_type_is_unknown(const msg_t *msg);
void msg_type_set_unknown(msg_t *msg);

int  msg_type_is_argument(const msg_t *msg);
void msg_type_set_argument(msg_t *msg);

int  msg_type_is_console_input(const msg_t *msg);
void msg_type_set_console_input(msg_t *msg);

int  msg_data_is_unknown(const msg_t *msg);
void msg_data_set_unknown(msg_t *msg);

void     msg_set_priority(msg_t *msg, uint32_t priority);
uint32_t msg_get_priority(const msg_t *msg);

void     msg_set_sender_pid(msg_t *msg, uint32_t sender_pid);
uint32_t msg_get_sender_pid(const msg_t *msg);

void     msg_set_receiver_pid(msg_t *msg, uint32_t receiver_pid);
uint32_t msg_get_receiver_pid(const msg_t *msg);

int         msg_data_is_string(const msg_t *msg);
void        msg_data_set_string(msg_t *msg, const char string[]);
const char *msg_data_get_string(const msg_t *msg);

int  msg_data_is_integer(const msg_t *msg);
void msg_data_set_integer(msg_t *msg, int integer);
int  msg_data_get_integer(const msg_t *msg);

msg_t *
create_string_message(char *str, uint32_t receiver_pid);

#endif
