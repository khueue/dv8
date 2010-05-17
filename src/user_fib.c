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
        msg_t *msg = NULL;
        uint32_t pid = exec("incr", PROCESS_DEFAULT_PRIORITY);
        kdebug_print("pid = ");
        kdebug_printint(pid);
        kdebug_println("");
        msg = create_string_message("nu kommer aRG", pid);
        msg_type_set_argument(msg);
        if (send_message(msg))
        {
            kdebug_println("send message JAPP");
        }
        else
        {
            kdebug_println("send message NEEEEEEEJ");
        }
    }

    while (1)
    {
        msg_t *msg = read_from_console();
        if (msg_data_is_string(msg))
        {
            if (0 == strcmp(msg_data_get_string(msg), "exit"))
            {
                kdebug_println("Killing fib now ...");
                kill_self();
            }
            kdebug_print("------ STRING FOR FIB: \"");
            kdebug_print(msg_data_get_string(msg));
            kdebug_println("\"");
        }
        else if (msg_data_is_integer(msg))
        {
            kdebug_print("------ INTEGER FOR FIB: ");
            kdebug_printint(msg_data_get_integer(msg));
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
