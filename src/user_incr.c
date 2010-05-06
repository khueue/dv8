#include "user_incr.h"
#include "utils.h"

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
    /*terminate_this_process();*/
    kdebug_println("end of incr");
    while (1)
    {
        static int count = 0;
        if ((count++ % 10000000) == 0)
        {
            kdebug_println("--- incr whiling ...");
        }
    }
    /* We should never get here! */
}
