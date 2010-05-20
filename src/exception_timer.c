#include "malta.h"
#include "utils.h"
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
        sch_decrease_sleep();
        sch_run();
    }
}
