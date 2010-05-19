#include "utils.h"
#include "pcb.h"
#include "msg.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * All PCBs available in the system, organised on a freelist.
 */
static pcb_t
g_pcbs[NUM_PCBS];

/*
 * Freelist pointer. Points to the first free PCB.
 */
static pcb_t *
g_freelist;

/*
 * ---------------------------------------------------------------------------
 * Private functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the freelist like a normal linked list.
 */
static void
pcb_init_freelist(void)
{
    size_t i = 0;

    g_freelist = &g_pcbs[0];

    for (i = 0; i < COUNT_ARRAY(g_pcbs)-1; ++i)
    {
        g_pcbs[i].next_free = &g_pcbs[i+1];
    }

    g_pcbs[COUNT_ARRAY(g_pcbs)-1].next_free = NULL;
}

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Assigns the given PCB a pid which is guaranteed to be unique among the
 * currently running processes. This implementation depends on the fact that
 * all PCBs are statically allocated as an array. This makes it possible to
 * base the pid calculation on the memory location of each PCB.
 */
void
pcb_assign_pid(pcb_t *pcb)
{
    /*
     * pcb[0] @ mem_offset 0
     * pcb[1] @ mem_offset 1*sizeof(pcb_t)
     * pcb[2] @ mem_offset 2*sizeof(pcb_t)
     * ...
     */
    size_t mem_offset = (size_t)pcb - (size_t)g_pcbs;

    /*
     * pcb[0].pid == (0 / sizeof(pcb_t)) + 1 == 1
     * pcb[0].pid == (1*sizeof(pcb_t) / sizeof(pcb_t)) + 1 == 2
     * pcb[0].pid == (2*sizeof(pcb_t) / sizeof(pcb_t)) + 1 == 3
     * ...
     */
    pcb->pid = (mem_offset / sizeof(*pcb)) + 1;
}

void
pcb_assign_supervisor(pcb_t *pcb, int supervisor_pid)
{
    pcb->supervisor_pid = supervisor_pid;
}

/*
 * XXXXXXX
 */
void
pcb_init(pcb_t *pcb)
{
    pcb_assign_pid(pcb);
    pcb_assign_supervisor(pcb, 0);
    prio_init(&pcb->inbox_q, msg_cmp_priority, msg_has_type);
}

/*
 * Compares the priority of two pcbs, returns:
 *   >0 : First argument has greater priority.
 *    0 : Equal.
 *   <0 : Second argument has greater priority.
 */
int
pcb_cmp_priority(const void *ppcb1, const void *ppcb2)
{
    const pcb_t *pcb1 = (const pcb_t *)ppcb1;
    const pcb_t *pcb2 = (const pcb_t *)ppcb2;

    kdebug_assert(pcb1);
    kdebug_assert(pcb2);

    return pcb1->priority - pcb2->priority;
}

/*
 * Compares the sleep left of two pcbs, returns:
 *   >0 : First argument has less sleep left.
 *    0 : Equal.
 *   <0 : Second argument has less sleep left.
 */
int
pcb_cmp_sleepleft(const void *ppcb1, const void *ppcb2)
{
    const pcb_t *pcb1 = (const pcb_t *)ppcb1;
    const pcb_t *pcb2 = (const pcb_t *)ppcb2;

    kdebug_assert(pcb1);
    kdebug_assert(pcb2);

    return pcb2->sleepleft - pcb1->sleepleft;
}

/*
 * Returns true if the pcb pointed to by ppcb has the same pid as the pid
 * pointed to by ppid.
 */
int
pcb_has_pid(const void *ppcb, const void *ppid)
{
    const pcb_t *pcb = NULL;
    const uint32_t *pid = NULL;

    kdebug_assert(ppcb);
    kdebug_assert(ppid);

    pcb = (const pcb_t *)ppcb;
    pid = (const uint32_t *)ppid;

    return pcb->pid == *pid;
}

int
pcb_has_supervisor(pcb_t *pcb)
{
    return pcb->supervisor_pid;
}

/*
 * Returns true if the pcb pointed to by pcb has sleep left 0 or less.
 */
int
pcb_is_done_sleeping(const pcb_t *pcb)
{
    kdebug_assert(pcb);

    return (pcb->sleepleft <= 0);
}

/*
 * Returns a free PCB, or NULL if none free.
 */
pcb_t *
pcb_alloc(void)
{
    static int is_initialized = 0;
    pcb_t *pcb = NULL;

    if (!is_initialized)
    {
        is_initialized = 1;
        pcb_init_freelist();
    }

    pcb = g_freelist;
    if (!pcb)
    {
        return NULL;
    }
    else
    {
        g_freelist = g_freelist->next_free;
        return ZERO_STRUCT(pcb);
    }
}

/* XXXXXXX */
size_t
pcb_get_inbox_length(pcb_t *pcb)
{
    return pcb->inbox_q.length;   
}
/*
 * Releases a PCB back to the system. Always returns NULL, to make it easy to
 * avoid dangling pointers by simply using the return value in an assignment.
 */
pcb_t *
pcb_free(pcb_t *pcb)
{
    msg_t *msg = NULL;

    kdebug_assert(pcb);

    while ((msg = prio_find_head(&pcb->inbox_q)))
    {
        msg_free(msg);
        prio_remove_head(&pcb->inbox_q);
    }

    pcb->next_free = g_freelist;
    g_freelist = pcb;

    return NULL;
}

void
pcb_print(const pcb_t *process)
{       
    
    kdebug_printint(process->pid);
    kdebug_print("\t");

    kdebug_printint(process->priority);
    kdebug_print("\t\t");

    kdebug_printint(process->inbox_q.length);
    kdebug_print("\t");

    if(process->state == PROCESS_STATE_RUNNING)
    {
        kdebug_print("RUN");
    }
    else if (process->state == PROCESS_STATE_READY)
    {
        kdebug_print("READY");
    }
    else if (process->state == PROCESS_STATE_SLEEPING)
    {
        kdebug_print("SLEEP");
    }
    else if (process->state == PROCESS_STATE_BLOCKED)
    {
        kdebug_print("BLOCKED");
    }
    else if (process->state == PROCESS_STATE_NEW)
    {
        kdebug_print("NEW");
    }
    else if (process->state == PROCESS_STATE_TERMINATED)
    {
        kdebug_print("TERMINATED");
    }
    else
    {
        kdebug_print("***** PROCESS_STATE_?????");
    }

    kdebug_print("\t");

    kdebug_printint(process->sleepleft);
    kdebug_println("");
}
/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DPCB_MAIN src/pcb.c \
    src/utils.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef PCB_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    pcb_t *pcb = NULL;

    pcb = pcb_alloc();
    pcb->priority = 1;
    printf("%d\n", pcb->priority);
    pcb = pcb_free(pcb);

    /* Allocate again, should be the same struct but zeroed. */
    pcb = pcb_alloc();
    printf("%d\n", pcb->priority);
    pcb = pcb_free(pcb);

    return 0;
}

#endif
