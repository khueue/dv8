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
    int pid_to_bully = exec("supervisor_demo", PROCESS_DEFAULT_PRIORITY);
    msg_t *msg = msg_alloc();
    msg_t *msg2 = msg_alloc();
    msg_set_receiver_pid(msg, pid_to_bully);
    msg_data_set_integer(msg, 666);
    int pid_of_dead = 0;
    int pcb_state = 0;
    while(1)
    {        
        // CHECK FOR MESSAGES
        read_message_by_type(msg2, MSG_TYPE_SUPERVISOR_NOTICE_ID, 1);
        pid_of_dead = msg_data_get_integer(msg2);
        read_message_by_type(msg2, MSG_TYPE_SUPERVISOR_NOTICE_STATE, 1);
        pcb_state = msg_data_get_integer(msg2);
        if (pid_of_dead != 0 && pcb_state != PROCESS_STATE_NEW)
        {
            if (pcb_state == PROCESS_STATE_TERMINATED) {
                kdebug_print("Process ");
                kdebug_printint(pid_of_dead);
                kdebug_println(" quit unexpectedly. Respawning...");
                pid_to_bully = exec("supervisor_demo", PROCESS_DEFAULT_PRIORITY);
            }
            if (pcb_state == PROCESS_STATE_ENDED) {
                kdebug_print("Process ");
                kdebug_printint(pid_of_dead);
                kdebug_println(" quit normally.");
            }
            msg_free(msg2);
            msg_set_receiver_pid(msg, pid_to_bully);
            pid_of_dead = 0;
            pcb_state = PROCESS_STATE_NEW;
        }
        else
        {
            send_message(msg);
        }
    }
    msg_free(msg);
}
