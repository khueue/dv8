#include "asm.h"
#include "malta.h"
#include "utils.h"
#include "pcb.h"
#include "scheduler.h"

#include "exception_timer.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
execute_timer(cause_reg_t cause)
{
    /* Make sure that we are here because of a timer interrupt. */
    if (cause.field.exc == 0) /* External interrupt. */
    {
        static int k = 0; /* XXXXXXXXXX */
        pcb_t *current = sch_get_current_running();
        
        if (k == 0)
        {
            k = 1;
            kset_registers(&current->regs);
        }
        else
        {
            pcb_t *next = NULL;
            memcpy(&current->regs, kget_registers(), sizeof(registers_t));
            next = sch_run();
            kset_registers(&next->regs);
        }

        /* Reload timer for another 100 ms (simulated time) */
        kload_timer(50 * timer_msec);
    }
}
