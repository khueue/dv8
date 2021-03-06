#include "scheduler.h"
#include "kernel.h"
#include "pcb.h"
#include "prio_queue.h"
#include "asm.h"
#include "msg.h"

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
    prio_init(&g_ready, pcb_cmp_priority,  pcb_has_pid);
    prio_init(&g_run,   pcb_cmp_priority,  pcb_has_pid);
    prio_init(&g_wait,  pcb_cmp_sleepleft, pcb_has_pid);
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
 * Tries to find a process in the scheduler and returns it.
 */
pcb_t *
sch_find_process(uint32_t pid)
{
    pcb_t *process = NULL;

    process = prio_find(&g_run, &pid);
    if (process)
    {
        return process;
    }

    process = prio_find(&g_ready, &pid);
    if (process)
    {
        return process;
    }

    process = prio_find(&g_wait, &pid);
    if (process)
    {
        return process;
    }

    return NULL;
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

void
sch_decrease_sleep(void)
{
    pcb_t *process = NULL;

    /*
     * Update wait queue and move anything ready to wake up back into ready
     */

    prio_iter_reset(&g_wait);
    while (prio_iter_has_next(&g_wait))
    {
        process = prio_iter_next(&g_wait);

        process->sleepleft -= MS_TO_NEXT_TIMER_INTERRUPT * timer_msec;
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

    prio_iter_reset(&g_wait);
    while (prio_iter_has_next(&g_wait))
    {
        process = prio_iter_next(&g_wait);

        if (pcb_is_done_sleeping(process) && (process->state != PROCESS_STATE_BLOCKED))
        {
            prio_remove(&g_wait, &process->pid);
            prio_iter_reset(&g_wait);

            process->state = PROCESS_STATE_READY;
            prio_enqueue(&g_ready, process);
        }
    }

    /*
     * If a process was using the CPU, move it from the run queue to the ready
     * queue.
     */
    process = sch_get_currently_running_process();
    if (process)
    {
        prio_dequeue(&g_run);
        process->state = PROCESS_STATE_READY;
        prio_enqueue(&g_ready, process);
    }

    /*
     * Fetch the next process to be run (from the ready queue) and prepare to
     * load its state into the CPU.
     */
    process = prio_dequeue(&g_ready);

    process->state = PROCESS_STATE_RUNNING;
    prio_enqueue(&g_run, process);
    restore_process_state(process);

    /* Reload interrupt timer. */
    kload_timer(MS_TO_NEXT_TIMER_INTERRUPT * timer_msec);
}

/*
 * Schedules a process for CPU usage.
 */

void
sch_schedule(pcb_t *pcb)
{
    pcb->state = PROCESS_STATE_READY;
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
     * If a process was using the CPU, move it from the run queue to the wait
     * queue.
     */
    process = sch_get_currently_running_process();
    if (process)
    {
        prio_dequeue(&g_run);
        process->state = PROCESS_STATE_SLEEPING;
        prio_enqueue(&g_wait, process);
    }
}

/*
 * Change priority
 */
uint32_t
sch_change_priority(uint32_t pid, uint32_t priority)
{
    pcb_t *process = NULL;

    process = prio_remove(&g_run, &pid);
    if (process)
    {
        process->priority = priority;
        prio_enqueue(&g_run, process);
        return 1;
    }

    process = prio_remove(&g_ready, &pid);
    if (process)
    {
        process->priority = priority;
        prio_enqueue(&g_ready, process);
        return 1;
    }

    process = prio_remove(&g_wait, &pid);
    if (process)
    {
        process->priority = priority;
        prio_enqueue(&g_wait, process);
        return 1;
    }

    return 0;
}

/*
 * Block process
 */
uint32_t
sch_block(uint32_t pid)
{
    pcb_t *process = NULL;

    process = prio_remove(&g_run, &pid);
    if (process)
    {
        process->state = PROCESS_STATE_BLOCKED;
        prio_enqueue(&g_wait, process);
        return 1;
    }

    process = prio_remove(&g_ready, &pid);
    if (process)
    {
        process->state = PROCESS_STATE_BLOCKED;
        prio_enqueue(&g_wait, process);
        return 1;
    }

    process = prio_remove(&g_wait, &pid);
    if (process)
    {
        process->state = PROCESS_STATE_BLOCKED;
        prio_enqueue(&g_wait, process);
        return 1;
    }

    return 0;
}

/*
 * Unblock process
 */
uint32_t
sch_unblock(uint32_t pid)
{
    pcb_t *process = NULL;

    process = prio_remove(&g_wait, &pid);
    if (process)
    {
        process->state = PROCESS_STATE_READY;
        prio_enqueue(&g_ready, process);

        return 1;
    }

    return 0;
}

/*
 * Remove process
 */
pcb_t *
sch_unschedule(uint32_t pid)
{
    pcb_t *process = NULL;

    process = prio_remove(&g_run, &pid);
    if (process)
    {
        return process;
    }

    process = prio_remove(&g_ready, &pid);
    if (process)
    {
        return process;
    }

    process = prio_remove(&g_wait, &pid);
    if (process)
    {
        return process;
    }

    return NULL;
}

static void
print_line(void)
{
    int i = 0;

    for ( i = 0; i < 78; i += 1 )
    {
        kprint_char('-');
    }
    kprint_strln("");
}

void
sch_print()
{
    pcb_t *process = NULL;

    kprint_str("Name");
    kprint_str("\t");
    kprint_str("Pid");
    kprint_str("\t");
    kprint_str("Priority");
    kprint_str("\t");
    kprint_str("Inbox");
    kprint_str("\t");
    kprint_str("State");
    kprint_str("\t");
    kprint_strln("Sleep(ms)");

    print_line();

    prio_iter_reset(&g_wait);
    while (prio_iter_has_next(&g_wait))
    {
        process = prio_iter_next(&g_wait);
        pcb_print(process);
    }

    prio_iter_reset(&g_ready);
    while (prio_iter_has_next(&g_ready))
    {
        process = prio_iter_next(&g_ready);
        pcb_print(process);
    }

    prio_iter_reset(&g_run);
    while (prio_iter_has_next(&g_run))
    {
        process = prio_iter_next(&g_run);
        pcb_print(process);
    }
    print_line();

    kprint_str("Num free messages: ");
    kprint_int(msg_num_free());
    kprint_strln("");

    kprint_str("Num free PCBs: ");
    kprint_int(pcb_num_free());
    kprint_strln("");

    kprint_str("Num free list nodes: ");
    kprint_int(list_node_num_free());
    kprint_strln("");

    print_line();
}
