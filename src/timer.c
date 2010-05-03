#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "utils.h"

#include "timer.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXX
 */
void
execute_timer(cause_reg_t cause)
{
    /* Make sure that we are here because of a timer interrupt. */
    if (cause.field.exc == 0) /* External interrupt. */
    {
        static int i = 0;

        /* Reload timer for another 100 ms (simulated time) */
        kload_timer(50 * timer_msec);

        /* Increase the number on the Malta display. */
        display_word(++i);
    }
}
