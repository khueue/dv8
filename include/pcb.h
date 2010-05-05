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
    uint8_t stack[0x4000];
    int32_t pid;
    int32_t priority;
    registers_t regs;

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

#endif
