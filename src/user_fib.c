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
    #if 0
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
    #endif

    #if 0
    while (1)
    {
        msg_t *msg = msg_alloc();
        read_message_by_type(msg, MSG_TYPE_CONSOLE_INPUT, 0);
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
    #endif

    msg_t *msg = msg_alloc();
    int n = 0;
    int i = 0;
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 330);
    if (msg_type_is_invalid(msg))
    {
        print_str("XXXXXXXXXXX USAGE FIB INVALID");
        return;
    }
    n = atoi(msg_data_get_string(msg));
    msg = msg_free(msg);
    if (n <= 0)
    {
        print_str("XXXXXXXXXXX USAGE FIB");
        return;
    }

    for (i = 0; i <= n; ++i)
    {
        kdebug_print("fib(");
        kdebug_printint(i);
        kdebug_print(") = ");
        kdebug_printint(fib_recursive(i));
        kdebug_println("");
        sleep(100);
    }
}
