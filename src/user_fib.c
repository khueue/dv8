#include "utils.h"
#include "kernel_api.h"

#include "user_fib.h"
#include "user_incr.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

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
fib(void)
{
    size_t i = 0;
    for (i = 0; i < 10; ++i)
    {
        kdebug_print("fib(");
        kdebug_printint(i);
        kdebug_print(") = ");
        kdebug_printint(fib_recursive(i));
        kdebug_println("");
    }
    
    {
        uint32_t pid = exec(incr);
        kdebug_print("pid = ");
        kdebug_printint(pid);
        kdebug_println("");
    }

    do_nothing_forever("fib doing nothing ...", 10000000);
}
