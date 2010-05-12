#include "utils.h"
#include "kernel_api.h"

#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"

#include "user_ring.h"
#include "user_ringnode.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */


void
ringnode(void)
{
    /*
    message = NULL;
    while(1)
    {
        message = read_message();
        if (!message)
        {
            block_self;
        }
        else if (dest_pid == 0)
        {
            dest_pid = message;
        }
        else
        {
            print(getpid);
            print(" recieved message ");
            print(message);
            print(" from ");
            println(sender);
            sleep(1000);
            send_message_and_awaken(dest_pid, message);
        }
    }
    */
}
