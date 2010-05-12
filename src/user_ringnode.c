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
ringnode(int dest_pid)
{
    int i = dest_pid;
    i++;
    /*
    char message[];
    while(!message)
    {
        block_self();
        message = read_message()
        if (message)
        {
            println(" recieved message ");
            println(message);
            println(" from ");
            println(sender);
            sleep(2000);
            send_with_wakeup(message, dest_pid);
        }
    }
    */
}
