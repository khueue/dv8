#include "utils.h"
#include "pcb_freelist.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

#ifndef PCB_MAX_LENGTH
#define PCB_MAX_LENGTH 20
#endif

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
static pcb_t
g_pcbs[PCB_MAX_LENGTH];

/*
 * XXX
 */
static pcb_t *
g_pcb_freelist;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
void
init_pcb_freelist(void)
{
    size_t i;

    g_pcb_freelist = &g_pcbs[0];

    for (i = 0; i < PCB_MAX_LENGTH - 1; i++)
    {
        g_pcbs[i].next_free = &g_pcbs[i + 1];
    }

    g_pcbs[PCB_MAX_LENGTH - 1].next_free = NULL;
}

/*
 * XXX
 */
pcb_t *
alloc_pcb(void)
{
    pcb_t *pcb = g_pcb_freelist;
    if (!pcb)
    {
        return NULL;
    }
    g_pcb_freelist = g_pcb_freelist->next_free;
    return ZERO_STRUCT(pcb);
}

/*
 * XXX
 */
void
free_pcb(pcb_t *pcb)
{
    pcb->next_free = g_pcb_freelist;
    g_pcb_freelist = pcb;
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
#ifdef PCB_FREELIST_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    pcb_t *pcb;

    init_pcb_freelist();

    pcb = alloc_pcb();
    pcb->priority = 1;
    printf("%d\n", pcb->priority);
    free_pcb(pcb);

    /* Allocate again, should be the same struct but zeroed. */
    pcb = alloc_pcb();
    printf("%d\n", pcb->priority);
    free_pcb(pcb);

    return 0;
}

#endif
