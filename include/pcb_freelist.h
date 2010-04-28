#ifndef PCB_FREELIST_H
#define PCB_FREELIST_H

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
struct pcb;
typedef struct pcb pcb_t;
struct pcb
{
   int priority;
   pcb_t *next_free;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

pcb_t *
alloc_pcb(void);

void
free_pcb(pcb_t *pcb);

void
init_pcb_freelist(void);

#endif
