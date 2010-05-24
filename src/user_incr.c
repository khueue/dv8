#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

#include "msg.h"

static void
print_usage(void)
{
    print_strln("usage: incr <int limit>");
}

void
incr(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    int n = 0;
    int i = 0;

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 200);
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
        sleep(200);

        print_int(i);
        print_strln("");
    }
}
