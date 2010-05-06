#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "utils.h"
#include "pcb.h"

#include "exception_timer.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXX
 */
static void
copy_registers(registers_t *dst, registers_t *src)
{
    uint8_t *pdst = (uint8_t *)dst;
    uint8_t *psrc = (uint8_t *)src;
    size_t i = 0;
    for (i = 0; i < sizeof(registers_t); ++i)
    {
        *pdst++ = *psrc++;
    }
}
 
void
execute_timer(cause_reg_t cause)
{
   
    /* Make sure that we are here because of a timer interrupt. */
    if (cause.field.exc == 0) /* External interrupt. */
    {
        static int i = 1;
        extern pcb_t *p1, *p2;
        registers_t *tmp;
        
        
        tmp = (i == 1) ? &p2->regs : kget_registers();        
        
        
        
        /* temp schedule */
        if(i == 2) 
        {
            copy_registers(&p1->regs, tmp);
            kset_registers(&p2->regs);
            i = 0;
        } 
        else 
        {
            copy_registers(&p2->regs, tmp);
            kset_registers(&p1->regs);
            i = 2;  
        }
        /* Reload timer for another 100 ms (simulated time) */
        kload_timer(50 * timer_msec);
   
    }
}
