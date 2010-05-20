#include "utils.h"
#include "kernel_api.h"

#include "scheduler.h" /* SUPER REMOVE XXXXXXXXX */
#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"
#include "settings.h"
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
    char spam_string[] = "Enlarge your penis";
    int inbox_limit = 30;
    int messages_to_send = 40;
    set_inbox_limit(inbox_limit);

    int i;

    for (i = 0; i < messages_to_send; i++)
    {
        msg_data_set_string(msg, spam_string);
        msg_set_receiver_pid(msg, getpid());
        send_message(msg);
    }
}
