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
    uint32_t pid;
    uint32_t priority;
    registers_t regs;
    uint8_t stack[0x4000];

    /* Internal freelist pointer. */
    pcb_t *next_free;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

pcb_t *
pcb_alloc(void);

pcb_t *
pcb_free(pcb_t *pcb);

int
pcb_has_pid(void *pcb, void *pid);

int
pcb_cmp_priority(void *p1, void *p2);

#endif
