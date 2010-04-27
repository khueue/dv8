#include <stdio.h>

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
    g_pcb_freelist = g_pcb_freelist->next;
    return pcb;
}

void
free_pcb(pcb_t *pcb) 
{
    pcb_t *next = g_pcb_freelist;
    g_pcb_freelist = pcb;
    g_pcb_freelist->next = next;    
}

void
init_pcb_freelist(void)
{
    size_t i;
    
    g_pcb_freelist = &g_pcbs[0];
    
    for (i = 0; i < PCB_MAX_LENGTH - 1; i++)
    {
        g_pcbs[i].next = &g_pcbs[i + 1];
    }

    g_pcbs[PCB_MAX_LENGTH - 1].next = NULL;
}

int 
main(int argc, char *argv[])
{
    pcb_t *pcb;
    init_pcb_freelist();
    pcb = alloc_pcb();
    pcb->priority = 1;
        
    printf("%d\n",pcb->priority);

    free_pcb(pcb);
    (void)argc;
    (void)argv;
    return 0;
}

