#include "utils.h"
#include "kernel_api.h"

#include "scheduler.h" /* SUPER REMOVE XXXXXXXXX */
#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"

#include "user_ring.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

static int
get_arg(msg_t *msg, int timeout)
{
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, timeout);
    if (msg_type_is_invalid(msg))
    {
        return 0;
    }
    return 1;
}

void
ringnode(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    int index;
    int pid;
    char the_msg[256];
    char usage[] = "Error: ringn is executed with 'ring'";

    //Get ringnode index
    if (!get_arg(msg, 200) || !msg_data_is_integer(msg))
    {
        print_strln(usage);
        return;
    }
    index = msg_data_get_integer(msg);

    //Get next pid
    if (!get_arg(msg, 200) || !msg_data_is_integer(msg))
    {
        print_strln(usage);
        return;
    }
    pid = msg_data_get_integer(msg);

    //Get message
    if (!get_arg(msg, 0) || !msg_data_is_string(msg))
    {
        print_strln(usage);
        return;
    }
    strcpy(the_msg, msg_data_get_string(msg));

    print_int(getpid());
    print_str(" received message ");
    print_str(the_msg);
    print_str(" from ");
    print_int(msg_get_sender_pid(msg));
    print_strln("");

    sleep(1500);

    //Send message to next
    msg_set_receiver_pid(msg, pid);
    msg_data_set_string(msg, the_msg);
    send_message(msg);

    //If first/last in the ring
    if (index == 0)
    {
        //Wait for the message again
        if (!get_arg(msg, 0) || !msg_data_is_string(msg))
        {
            print_strln("Error: Failed to get final message.");
        }
        strcpy(the_msg, msg_data_get_string(msg));

        kdebug_printint(getpid());
        print_str(" received message ");
        print_str(the_msg);
        print_str(" from ");
        print_int(msg_get_sender_pid(msg));
        print_strln("\nEND OF RING");
    }
}

void
ring(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    int n = 0;
    int i = 0;
    int pids[NUM_PCBS];
    char the_msg[256];
    char usage[] = "Usage: ring [int nodes] [str message]";

    /* Get argument 1 - the number of nodes */
    if (!get_arg(msg, 300) || !msg_data_is_string(msg))
    {
        print_strln(usage);
        return;
    }

    n = atoi(msg_data_get_string(msg));

    /* Get argument 2 - the message*/
    if (!get_arg(msg, 300) || !msg_data_is_string(msg))
    {
        print_strln(usage);
        return;
    }
    strcpy(the_msg, msg_data_get_string(msg));

    msg_type_set_argument(msg);

    //Init pids
    for (i = 0; i < n; i++)
    {
        pids[i] = exec("ringn", PROCESS_DEFAULT_PRIORITY);
        if (!pids[i])
        {
            print_strln("Error: Failed to spawn the whole ring.");
            return;
        }
    }

    /* send index to first node */
    msg_set_receiver_pid(msg, pids[0]);
    msg_data_set_integer(msg, 0);
    send_message(msg);

    for (i = 1; i < n; i++)
    {
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

    /* Send message to first that will start the ring. */
    msg_set_receiver_pid(msg, pids[0]);
    msg_data_set_string(msg, the_msg);
    send_message(msg);
}
