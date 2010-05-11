#ifndef PCB_FREELIST_H
#define PCB_FREELIST_H

#include "registers.h"
#include "fifo_queue.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Process Control Block. Contains a process' registers and stack, and
 * anything else relevant to the process.
 */
typedef struct pcb pcb_t;
struct pcb
{
    uint32_t pid;
    uint32_t priority;
    int32_t sleepleft;
    uint8_t is_blocked;
    fifo_queue_t inbox_q;
    registers_t regs;
    uint8_t stack[PROCESS_STACK_SIZE];

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

void
pcb_assign_pid(pcb_t *pcb);

void
pcb_init(pcb_t *pcb);

int
pcb_has_pid(void *ppcb, void *ppid);

int
pcb_cmp_priority(void *pcb1, void *pcb2);

int
pcb_cmp_sleepleft(void *pcb1, void *pcb2);

int
pcb_is_done_sleeping(pcb_t *pcb);

#endif
