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
    /*char spam_string[] = "Enlarge your penis";
    int inbox_limit = 15;

    msg_t *msg = create_string_message(spam_string, getpid());
    
    int i;*/
    
    kdebug_println("Hejsan");
    
    /*for (i = 0; i < inbox_limit; i++)
    {
        kdebug_println("Spamming self...");
        send_message(msg);
        sleep(100);
    }*/
}
