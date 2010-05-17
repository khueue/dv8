#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

#include "msg.h"

void
incr(void)
{
    int i;
    msg_t msg;
    int success = 0;

    kdebug_println("-------------- incr startart");
    success = read_message_by_type(&msg, MSG_TYPE_ARGUMENT, 0);

    if (success)
    {
        kdebug_println("***** WE GOT THE ARGUMENT *****");
        kdebug_println(msg.data.string);
    }
    else
    {
        kdebug_println("***** NO MESSAGE IN 300 MS!!!!! *****");
    }

    for(i = 0; i < 5; i += 1 )
    {
        kdebug_printint(i);
        kdebug_println(" - Incr sleeping ...");
        sleep(300);
    }
}
