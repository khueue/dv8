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
        /* Run schedular */
        sch_run();

        /* Reload timer for another 100 ms (simulated time) */
        kload_timer(50 * timer_msec);
    }
}
