#include "utils.h"
#include "kernel_api.h"

#include "user_fib.h"
#include "user_incr.h"

int
fib_recursive(int n)
{
    if (n <= 1)
    {
        return 1;
    }
    else
    {
        return fib_recursive(n-1) + fib_recursive(n-2);
    }
}

void
fib_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    int n = 5;
    /* TODO: Print to tty.
    printf("fib(5) = %d\n", fib_recursive(n)); */
    kdebug_print("fib_recursive(5) = ");
    kdebug_printint(fib_recursive(n));
    kdebug_println("");
}

void
fib(void)
{
    kdebug_println("start of fib");
    fib_impl();
    /*terminate_this_process();*/
    kdebug_println("end of fib");

    {
        uint32_t pid = exec(incr);
        kdebug_print("pid = ");
        kdebug_printint(pid);
        kdebug_println("");
    }

    while (1)
    {
        static int count = 0;
        if ((count++ % 10000000) == 0)
        {
            kdebug_println("- fib whiling ...");
        }
    }
    /* We should never get here! */
}
