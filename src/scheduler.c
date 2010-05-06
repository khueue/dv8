#include "scheduler.h"
#include "pcb.h"
#include "prio_queue.h"

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

pcb_t *
sch_get_current_running(void)
{
    return g_run.head->data;
}

void
sch_init(void)
{
    prio_init_queue(&g_ready, pcb_cmp_priority, pcb_has_pid);
    prio_init_queue(&g_run,   pcb_cmp_priority, pcb_has_pid);
    prio_init_queue(&g_wait,  pcb_cmp_priority, pcb_has_pid);
}

pcb_t *
sch_run(void)
{
    pcb_t *prev = NULL;
    pcb_t *next = NULL;

    prev = prio_dequeue(&g_run);
    prio_enqueue(&g_ready, prev);

    next = prio_dequeue(&g_ready);
    prio_enqueue(&g_run, next);

    return next;
}

void
sch_schedule(pcb_t *pcb)
{
    prio_enqueue(&g_ready, pcb);
}
