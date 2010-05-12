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
ring(int number_of_nodes)
{
    int i = number_of_nodes;
    i++;
    /* int i first_pid, dest_pid;
    first_pid = dest_pid = exec(ringnode, 0);
    
    for (i = 0; i < number_of_nodes; i++)
    {
        dest_pid = exec(ringnode, dest_pid);
    }
    nån kod som ställer in dest_pid i den första noden
    
    skicka meddelande till första noden */
}
