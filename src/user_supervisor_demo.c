#include "utils.h"
#include "kernel_api.h"
#include "msg.h"

#include "user_supervisor_demo.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
supervisor_demo(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    uint32_t pid_of_dead = 0;
    int pcb_state = PROCESS_STATE_NEW;
    uint32_t pid_to_supervise = exec("spammer", PROCESS_DEFAULT_PRIORITY);

    if (!pid_to_supervise)
    {
        print_strln("Could not execute spammer!");
        return;
    }

    supervise(pid_to_supervise);
    int unexpected = 1;
    int tries = 0;

    while (unexpected && tries++ < 10)
    {
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_ID, 0);
        if (msg_type_is_invalid(msg))
        {
            print_strln("Did not receive proper pid!");
            return;
        }
        pid_of_dead = msg_data_get_integer(msg);
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_STATE, 0);
        if (msg_type_is_invalid(msg))
        {
            print_strln("Did not receive proper process state!");
            return;
        }
        pcb_state = msg_data_get_integer(msg);
        sleep(10);
        if (pcb_state == PROCESS_STATE_TERMINATED) {
            print_str("Process ");
            print_int(pid_of_dead);
            print_strln(" quit unexpectedly. Respawning...");
            pid_to_supervise = exec("spammer", PROCESS_DEFAULT_PRIORITY);
            if (!pid_to_supervise)
            {
                print_strln("Could not execute spammer!");
                return;
            }
            supervise(pid_to_supervise);
        }
        else if (pcb_state == PROCESS_STATE_ENDED) {
            print_str("Process ");
            print_int(pid_of_dead);
            print_strln(" quit normally.");
            unexpected = 0;
        }
        else
        {
            print_strln("Unhandled process state!");
        }
    }

    print_strln("Giving up ...");
}
