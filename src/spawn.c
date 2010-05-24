#include "utils.h"
#include "pcb.h"
#include "kernel_api.h"

#include "spawn.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

pcb_t *
spawn(const char name[], user_program_pointer program, uint32_t priority)
{
    pcb_t *pcb = pcb_alloc();

    if (!pcb)
    {
        print_str("Could not allocate new PCB!");
        return NULL;
    }

    pcb_init(pcb);

    strcpy(pcb->program, name);

    pcb->priority  = priority;
    pcb->sleepleft = 0;
    pcb->state     = PROCESS_STATE_NEW;

    pcb->regs.epc_reg = (uint32_t)program;
    pcb->regs.sp_reg  = (uint32_t)(pcb->stack + sizeof(pcb->stack) - 32);
    pcb->regs.gp_reg  = GLOBAL_DATA_OFFSET;
    pcb->regs.ra_reg  = (uint32_t)kill_self;

    return pcb;
}
