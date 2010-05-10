#include "scheduler.h"
#include "kernel.h"
#include "pcb.h"
#include "prio_queue.h"
#include "asm.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * The ready queue. Contains processes wanting the CPU.
 */
static prio_queue_t
g_ready;

/*
 * The run queue. Contains processes currently using the CPU. (In our case,
 * just one process can run at a time, since we have just one CPU.)
 */
static prio_queue_t
g_run;

/*
 * The wait queue. Contains processes waiting for something to happen, like
 * waking up from sleep or receiving input.
 */
static prio_queue_t
g_wait;

/*
 * ---------------------------------------------------------------------------
 * Private functions.
 * ---------------------------------------------------------------------------
 */

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes all queues used by the scheduler to use PCBs.
 */
void
sch_init(void)
{
    prio_init_queue(&g_ready, pcb_cmp_priority,  pcb_has_pid);
    prio_init_queue(&g_run,   pcb_cmp_priority,  pcb_has_pid);
    prio_init_queue(&g_wait,  pcb_cmp_sleepleft, pcb_has_pid);
}

/*
 * Removes the given process from the run queue.
 */
void
sch_remove_from_run(pcb_t *pcb)
{
    prio_remove(&g_run, &pcb->pid);
}

/*
 * Returns the process currently in the run queue, or NULL if queue is empty.
 */
pcb_t *
sch_get_currently_running_process(void)
{
    if (g_run.head)
    {
        return g_run.head->data;
    }
    else
    {
        return NULL;
    }
}

/*
 * Swaps out the currently running process and swaps in a new one. Must be run
 * within an exception (such as a timer interrupt or a syscall) to properly
 * switch CPU context since the exception handler does the actual register
 * swapping.
 */
void
sch_run(void)
{
    pcb_t *process = NULL;

    /*
     * Update wait queue and move anything ready to wake up back into ready
     */

    prio_iterator_reset(&g_wait);
    while (prio_iterator_has_next(&g_wait))
    {
        process = (pcb_t *)prio_iterator_next(&g_wait);

        process->sleepleft -= 50 * timer_msec;
    }

    prio_iterator_reset(&g_wait);
    while (prio_iterator_has_next(&g_wait))
    {
        process = (pcb_t *)prio_iterator_next(&g_wait);

        if (pcb_is_done_sleeping(process) && !process->is_blocked)
        {
            kdebug_printint(process->pid);
            kdebug_println("");
            prio_remove(&g_wait, &process->pid);
            prio_iterator_reset(&g_wait);
            
            prio_enqueue(&g_ready, process);           
        }        
    }

    /*
     * to the ready queue.
     */
    process = sch_get_currently_running_process();
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

    /* Reload timer for another 100 ms (simulated time) */
    kload_timer(50 * timer_msec);
}

/*
 * Schedules a process for CPU usage.
 */

void
sch_schedule(pcb_t *pcb)
{
    prio_enqueue(&g_ready, pcb);
}

/*
 * Lulls a process to sleep.
 */

void
sch_sleep(void)
{
    pcb_t *process = NULL;

    /*
     * If a process was using the CPU, save it and move it from the run queue
     * to the wait queue.
     */
    process = sch_get_currently_running_process();
    if (process)
    {
        save_process_state(process);
        prio_dequeue(&g_run);
        prio_enqueue(&g_wait, process);
    }
}
