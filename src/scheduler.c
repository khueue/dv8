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

static void
save_process_state(pcb_t *pcb)
{
    memcpy(&pcb->regs, kget_registers(), sizeof(pcb->regs));
}

static void
restore_process_state(pcb_t *pcb)
{
    kset_registers(&pcb->regs);
}

void
sch_run(void)
{
    pcb_t *process = sch_get_current_running();

    /*
     * If a process was using the CPU, save it and move it from the run queue
     * to the ready queue.
     */
    if (process)
    {
        save_process_state(process);
        prio_dequeue(&g_run);
        prio_enqueue(&g_ready, process);
    }

    /*
     * Fetch the next process to be run (from the ready queue) and prepare to
     * load its state into the CPU.
     */
    process = prio_dequeue(&g_ready);
    prio_enqueue(&g_run, process);
    restore_process_state(process);
}

void
sch_schedule(pcb_t *pcb)
{
    prio_enqueue(&g_ready, pcb);
}
