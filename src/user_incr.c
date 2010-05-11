#include "user_incr.h"
#include "utils.h"
#include "kernel_api.h"
#include "scheduler.h"

void
incr_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    static int i = 0;
    /* TODO: Print to tty. */
    kdebug_print("i = ");
    kdebug_printint(i++);
    kdebug_println("");
}

void
incr(void)
{
    kdebug_println("start of incr");
    incr_impl();
    /*terminate_this_process();*/
    if(change_priority(776776,99))
    {
        kdebug_println("Changed");
    } else {
        kdebug_println("Failed to change");
    }
    
    
    
    while (1)
    {
        kdebug_println("Incr zombieing");
       
    }
    /* We should never get here! */
}
