#include "utils.h"
#include "asm.h"
#include "malta.h"

#include "pcb.h"
#include "spawn.h"
#include "kernel_api.h"
#include "kernel.h"

void
terminate_this_process(void)
{
    /* What does this do? */
    kdebug_println("terminating process");
}

void
kfunc_to_go_to_when_the_process_ends_normally(void)
{
    kill_self();
    while(1)
    {
        kdebug_println("Nej nej!");
    }
    
}

pcb_t *
spawn(user_prog_pointer program)
{
    pcb_t *pcb = pcb_alloc();
    static uint32_t next_pid = 0;
    
    if (!pcb)
    {
        /* XXX no more pcbs! */
        return NULL;
    }

    pcb->pid = ++next_pid;
    pcb->priority = 20;
    pcb->sleepleft = 0;
    pcb->is_blocked = 0;

    pcb->regs.epc_reg = (uint32_t)program;

    pcb->regs.sp_reg  =
        (uint32_t)(pcb->stack + sizeof(pcb->stack) - 32); /* -32 ???? XXX */

#define GLOBAL_DATA_OFFSET 0x80000000
    pcb->regs.gp_reg  = GLOBAL_DATA_OFFSET;

    pcb->regs.ra_reg  =
        (uint32_t)kfunc_to_go_to_when_the_process_ends_normally;

    return pcb;
}
