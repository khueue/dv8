#ifndef PCB_FREELIST_H
#define PCB_FREELIST_H

#ifndef PCB_MAX_LENGTH
#define PCB_MAX_LENGTH 20
#endif

struct pcb;
typedef struct pcb pcb_t;
struct pcb
{
   int priority;
   pcb_t *next;
};

pcb_t *
alloc_pcb(void);

void
free_pcb(pcb_t *pcb);

void
init_pcb_freelist(void);

#endif
