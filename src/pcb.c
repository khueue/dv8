#include "utils.h"
#include "pcb.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * Maximum number of PCBs available in the system.
 */
#ifndef NUM_PCBS
#define NUM_PCBS 20
#endif

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
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXXX
 */
int
pcb_cmp_priority(void *p1, void *p2)
{
    pcb_t *pcb1 = (pcb_t *)p1;
    pcb_t *pcb2 = (pcb_t *)p2;

    return pcb1->priority - pcb2->priority;
}

/*
 * XXXXXX
 */
int
pcb_has_pid(void *ppcb, void *ppid)
{
    pcb_t *pcb = (pcb_t *)ppcb;
    uint32_t pid = *(uint32_t *)ppid;

    return pcb->pid == pid;
}

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

/*
 * Releases a PCB back to the system. Always returns NULL, to make it easy and
 * idiomatic to avoid dangling pointers:
 *
 *   pcb = pcb_alloc();
 *   ... use pcb ...
 *   pcb = pcb_free();
 */
pcb_t *
pcb_free(pcb_t *pcb)
{
    pcb->next_free = g_freelist;
    g_freelist = pcb;
    return NULL;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
 * Define this constant and compile this and required modules, e.g.:
 *   gcc <this_module>.c <other_modules>.c -D<THIS_MODULE>_MAIN -Iinclude
 *   ./a.out
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
