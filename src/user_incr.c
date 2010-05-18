#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

#include "msg.h"

void
incr(void)
{
    msg_t *msg = msg_alloc();
    int n = 0;
    int i = 0;
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 330); /* XXXXX settings.h */
    if (msg_type_is_invalid(msg))
    {
        print_strln("XXXXXXXXXXX USAGE incr INVALID");
        return;
    }
    n = atoi(msg_data_get_string(msg));
    msg = msg_free(msg);
    if (n <= 0)
    {
        print_strln("XXXXXXXXXXX USAGE incr");
        return;
    }

    for (i = 0; i <= n; ++i)
    {
        print_int(i);
        print_strln("");
        sleep(300);
    }
}
