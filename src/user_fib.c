#include "utils.h"
#include "kernel_api.h"

#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"

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
    msg_t *msg = NULL;
    
    /* Spawn an increment, just for fun. */
    {
        uint32_t pid = exec(incr);
        kdebug_print("pid = ");
        kdebug_printint(pid);
        kdebug_println("");
    }

    msg = read_inbox_message();
    kdebug_print("------ MESSAGE FOR FIB: ");
    kdebug_println(msg->data.string);

    size_t i = 0;
    for (i = 0; i < 10; ++i)
    {
        kdebug_print("fib(");
        kdebug_printint(i);
        kdebug_print(") = ");
        kdebug_printint(fib_recursive(i));
        kdebug_println("");
        sleep(1002);

    }

}
