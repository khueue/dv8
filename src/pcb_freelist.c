#include "utils.h"
#include "pcb_freelist.h"

static pcb_t
g_pcbs[PCB_MAX_LENGTH];

static pcb_t *
g_pcb_freelist;

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

void
free_pcb(pcb_t *pcb)
{
    pcb->next_free = g_pcb_freelist;
    g_pcb_freelist = pcb;
}

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
