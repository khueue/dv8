#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

void
incr(void)
{
    while (1)
    {
        kdebug_println("Incr sleeping ...");
        sleep(300);
    }
}
