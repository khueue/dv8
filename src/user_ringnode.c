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
ringnode(void)
{
    msg_t *msg = msg_alloc();
    int index;
    int pid;
    char the_msg[256];
    char usage[] = "Error: ringnode is executed with 'ring'";

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 300);
    if (msg_type_is_invalid(msg))
    {
        print_strln(usage);
        msg = msg_free(msg);
        return;
    }

    if (msg_data_is_integer(msg))
    {
        index = msg_data_get_integer(msg);
    }
    else
    {
        print_strln(usage);
    }

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 300);
    if (msg_type_is_invalid(msg))
    {
        print_strln(usage);
        msg = msg_free(msg);
        return;
    }

    if (msg_data_is_integer(msg))
    {
        pid = msg_data_get_integer(msg);
    }
    else
    {
        print_strln(usage);
    }

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 0);
    if (msg_type_is_invalid(msg))
    {
        print_str("FAIL\n");
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
    }

    print_int(getpid());
    print_str(" received message ");
    print_str(the_msg);
    print_str(" from ");
    print_int(msg_get_sender_pid(msg));
    print_strln("");

    sleep(300);
    msg_set_receiver_pid(msg, pid);
    msg_data_set_string(msg, the_msg);
    send_message(msg);
    if(index == 0)
    {
        read_message_by_type(msg, MSG_TYPE_ARGUMENT, 0);
        if (msg_type_is_invalid(msg))
        {
            kdebug_println("No string!");
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
        }

        kdebug_printint(getpid());
        print_str(" received message ");
        print_str(the_msg);
        print_str(" from ");
        print_int(msg_get_sender_pid(msg));
        print_strln("\nEND OF RING");

    }
    msg = msg_free(msg);
}
