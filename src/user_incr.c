#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"

void
incr_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    static int i = 0;
    /* TODO: Print to tty. */
    kdebug_print("i = ");
    kdebug_printint(i++);
    kdebug_println("");
}

void
incr(void)
{
    kdebug_println("start of incr");
    incr_impl();
    kdebug_println("end of incr");
    while (1)
    {
        kdebug_println("Incr zombieing");
        sleep(700);
    }
    /* We should never get here! */
}
