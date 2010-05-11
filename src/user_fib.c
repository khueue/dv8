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
    /* Spawn an increment, just for fun. */
    {
        uint32_t pid = exec(incr);
        kdebug_print("pid = ");
        kdebug_printint(pid);
        kdebug_println("");
    }

    while (1)
    {
        msg_t *msg = read_from_console();
        if (msg->data_type == MSG_DATA_TYPE_STRING)
        {
            kdebug_print("------ STRING FOR FIB: \"");
            kdebug_print(msg->data.string);
            kdebug_println("\"");
        }
        else if (msg->data_type == MSG_DATA_TYPE_INTEGER)
        {
            kdebug_print("------ INTEGER FOR FIB: ");
            kdebug_printint(msg->data.integer);
            kdebug_println("");
        }
        else
        {
            /* Unhandled data type. */
        }
        msg = msg_free(msg);
    }

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
