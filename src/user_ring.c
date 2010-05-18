#include "utils.h"
#include "kernel_api.h"

#include "scheduler.h" /* SUPER REMOVE XXXXXXXXX */
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
ring(void)
{
    msg_t *msg = msg_alloc();
    int n = 0;
    int i = 0;
    int pids[10];
    char the_msg[256];
    
    /* Get argument 1 */
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 300);
    if (msg_type_is_invalid(msg))
    {
        print_str("XXXXXXXXXXX USAGE RING INVALID");
        msg = msg_free(msg);
        return;
    }
    n = atoi(msg_data_get_string(msg));
    
    if (n <= 0 || n > 10)
    {
        print_str("XXXXXXXXXXX USAGE RING");
        msg = msg_free(msg);
        return;
    }
    
    /* Get argument 2 */
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 200);
    if (msg_type_is_invalid(msg))
    {
        print_strln("FAIL");
        msg = msg_free(msg);
        return;
    }

    if (msg_data_is_string(msg))
    {
        strcpy(the_msg, msg_data_get_string(msg));
    }
    else
    {
        kdebug_println("No string!");
        print_str("XXXXXXXXXXX USAGE RING");
        msg = msg_free(msg);
        return;
    }
    
    
    msg_type_set_argument(msg);
    
    pids[i] = exec("ringnode", PROCESS_DEFAULT_PRIORITY);
    
    /* send index */
    msg_set_receiver_pid(msg, pids[i]);
    msg_data_set_integer(msg, i); 
    send_message(msg);    

    
    for (i = 1; i < n; i++)
    {
        pids[i] = exec("ringnode", PROCESS_DEFAULT_PRIORITY);
        
        /* send index */
        msg_set_receiver_pid(msg, pids[i]);
        msg_data_set_integer(msg, i); 
        send_message(msg); 
        
        /* send pids[i] to pids[i-1] */
        msg_set_receiver_pid(msg, pids[i-1]);
        msg_data_set_integer(msg, pids[i]);
        send_message(msg);    

        
    }

    /* Last should send to first in ring */
    msg_set_receiver_pid(msg, pids[i-1]);
    msg_data_set_integer(msg, pids[0]);
    send_message(msg);

    sch_print();
    
    /* Send message to first that will start the ring. */
    msg_set_receiver_pid(msg, pids[0]);
    msg_data_set_string(msg, the_msg);
    send_message(msg);
    msg = msg_free(msg);
}
