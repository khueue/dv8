#include "utils.h"
#include "kernel_api.h"

#include "scheduler.h" /* SUPER REMOVE XXXXXXXXX */
#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"
#include "settings.h"
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
        kdebug_println("xxxxxxxxxx ----------- HOLY SHIT");
        return;
    }

    supervise(pid_to_supervise);
    int unexpected = 1;

    while (unexpected)
    {
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_ID, 0);
        if (msg_type_is_invalid(msg))
        {
            kdebug_println("------ xxxxxHOLY COW!!!!");
            return;
        }
        pid_of_dead = msg_data_get_integer(msg);
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_STATE, 0);
        if (msg_type_is_invalid(msg))
        {
            kdebug_println("------ xxxxxx HOLY COW!!!!");
            return;
        }
        pcb_state = msg_data_get_integer(msg);
        sleep(300);
        if (pcb_state == PROCESS_STATE_TERMINATED) {
            kdebug_print("Process ");
            kdebug_printint(pid_of_dead);
            kdebug_println(" quit unexpectedly. Respawning...");
            pid_to_supervise = exec("spammer", PROCESS_DEFAULT_PRIORITY);
            if (!pid_to_supervise)
            {
                kdebug_println("------not pid  xxxxxx HOLY COW!!!!");
                return;
            }
            supervise(pid_to_supervise);
        }
        else if (pcb_state == PROCESS_STATE_ENDED) {
            kdebug_print("Process ");
            kdebug_printint(pid_of_dead);
            kdebug_println(" quit normally.");
            unexpected = 0;
        }
        else
        {
            kdebug_print("*Schnabelherstellungsfehler*");
        }
    }
}
