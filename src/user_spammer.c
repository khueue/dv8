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
    char spam_string[] = "Enlarge your penis";
    int inbox_limit = 30;
    int messages_to_send = 40;
    set_inbox_limit(inbox_limit);

    msg_t *msg = create_string_message(spam_string, getpid());

    int i;

    for (i = 0; i < messages_to_send; i++)
    {
        send_message(msg);
    }

    msg = msg_free(msg);
}
