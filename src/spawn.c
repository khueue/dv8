#include "utils.h"
#include "asm.h"
#include "malta.h"

#include "pcb.h"
#include "spawn.h"
#include "kernel_api.h"
#include "kernel.h"
#include "tty_manager.h"

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
spawn(user_program_pointer program, uint32_t priority)
{
    pcb_t *pcb = pcb_alloc();

    if (!pcb)
    {
        /* XXX no more pcbs! */
        return NULL;
    }

    pcb_init(pcb);

    pcb->priority = priority;
    pcb->sleepleft = 0;
    pcb->state = PROCESS_STATE_NEW;

    pcb->regs.epc_reg = (uint32_t)program;

    pcb->regs.sp_reg  =
        (uint32_t)(pcb->stack + sizeof(pcb->stack) - 32); /* -32 ???? XXX */

    pcb->regs.gp_reg  = GLOBAL_DATA_OFFSET;

    pcb->regs.ra_reg  =
        (uint32_t)kfunc_to_go_to_when_the_process_ends_normally;

    return pcb;
}
