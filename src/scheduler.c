#include "scheduler.h"
#include "pcb.h"
#include "prio_queue.h"
#include "asm.h"

static prio_queue_t
g_ready;

static prio_queue_t
g_wait;

static prio_queue_t
g_run;

void
sch_place_in_run(pcb_t *pcb)
{
    prio_enqueue(&g_run, pcb);
}

void
sch_remove_from_run(pcb_t *pcb)
{
    prio_remove(&g_run, &pcb->pid);
}

pcb_t *
sch_get_current_running(void)
{
    if(g_run.head)
        return g_run.head->data;
    else
        return NULL;
}

void
sch_init(void)
{
    prio_init_queue(&g_ready, pcb_cmp_priority, pcb_has_pid);
    prio_init_queue(&g_run,   pcb_cmp_priority, pcb_has_pid);
    prio_init_queue(&g_wait,  pcb_cmp_priority, pcb_has_pid);
}

void
sch_run(void)
{
    pcb_t *prev = NULL;
    pcb_t *next = NULL;

    pcb_t *current = sch_get_current_running();

    if(current) 
    {
        memcpy(&current->regs, kget_registers(), sizeof(registers_t));
        prev = prio_dequeue(&g_run);
        prio_enqueue(&g_ready, prev);
    }

    next = prio_dequeue(&g_ready);
    prio_enqueue(&g_run, next);
    kset_registers(&next->regs);    
}

void
sch_schedule(pcb_t *pcb)
{
    prio_enqueue(&g_ready, pcb);
}
