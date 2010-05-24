#include "utils.h"
#include "kernel_api.h"
#include "msg.h"

#include "user_spammer.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
spammer(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    char spam_string[] = "!!! SPAM MESSAGE !!!";
    int inbox_limit = 30;
    int messages_to_send = 40;
    set_inbox_limit(inbox_limit);

    int i = 0;

    msg_data_set_string(msg, spam_string);
    msg_set_receiver_pid(msg, getpid());
    for (i = 0; i < messages_to_send; i++)
    {
        send_message(msg);
    }
}
