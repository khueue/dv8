#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

void
incr(void)
{
    int i;
    for(i = 0; i < 5; i += 1 )
    {
        kdebug_printint(i);
        kdebug_println(" - Incr sleeping ...");
        sleep(300);
    }
}
