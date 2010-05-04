#ifndef PCB_FREELIST_H
#define PCB_FREELIST_H

#include "registers.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
typedef struct pcb pcb_t;
struct pcb
{
    int32_t pid;
    int32_t priority;
    registers_t regs;
    uint8_t stack[0x4000]; /* 16 kB. */

    /* Internal freelist pointer. */
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
