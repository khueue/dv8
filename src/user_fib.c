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

static void
print_usage(void)
{
    print_strln("");
    print_strln("Usage: fib [int limit]");
}

void
fib(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    int n = 0;
    int i = 0;

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 330);
    if (msg_type_is_invalid(msg))
    {
        print_usage();
        return;
    }

    n = atoi(msg_data_get_string(msg));
    if (n <= 0)
    {
        print_usage();
        return;
    }

    for (i = 0; i <= n; ++i)
    {
        print_str("fib(");
        print_int(i);
        print_str(") = ");
        print_int(fib_recursive(i));
        print_strln("");
        sleep(100);
    }
    print_strln(get_process_name(4));
    print_int(get_process_priority(4));
    print_strln("");
    print_int(get_process_state(4));
    print_strln("");
}
