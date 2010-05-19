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
    int pid_to_supervise = exec("spammer", PROCESS_DEFAULT_PRIORITY);
    msg_t *msg = msg_alloc();
    msg_type_set_argument(msg);
    send_message(msg);
    
    int pid_of_dead = 0;
    int pcb_state = PROCESS_STATE_NEW;
    while(1)
    {        
        // CHECK FOR MESSAGES
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_ID, 0;
        pid_of_dead = msg_data_get_integer(msg);
        read_message_by_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_STATE, 0);
        pcb_state = msg_data_get_integer(msg);
        if (pid_to_supervise == pid_of_dead && pcb_state != PROCESS_STATE_NEW)
        {
            if (pcb_state == PROCESS_STATE_TERMINATED) {
                kdebug_print("Process ");
                kdebug_printint(pid_of_dead);
                kdebug_println(" quit unexpectedly. Respawning...");
                pid_to_supervise = exec("spammer", PROCESS_DEFAULT_PRIORITY);
            }
            if (pcb_state == PROCESS_STATE_ENDED) {
                kdebug_print("Process ");
                kdebug_printint(pid_of_dead);
                kdebug_println(" quit normally.");
            }
        }
    }
    msg_free(msg);
}
