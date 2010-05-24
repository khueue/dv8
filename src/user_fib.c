#include "utils.h"
#include "kernel_api.h"
#include "msg.h"

#include "user_fib.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

static void
print_usage(void)
{
    print_strln("usage: fib <int limit>");
}

static int
fib_rec(int n)
{
    return (n <= 1) ? 1 : fib_rec(n-1) + fib_rec(n-2);
}

void
fib(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    int n = 0;
    int i = 0;

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 100);
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
        sleep(100);

        print_str("fib(");
        print_int(i);
        print_str(") = ");
        print_int(fib_rec(i));
        print_strln("");
    }
}
