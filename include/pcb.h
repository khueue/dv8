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
 * Process state
 */
typedef enum
{
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_READY,
    PROCESS_STATE_SLEEPING,
    PROCESS_STATE_BLOCKED,
    PROCESS_STATE_NEW,
    PROCESS_STATE_TERMINATED
} process_state_t;

/*
 * Process Control Block. Contains a process' registers and stack, and
 * anything else relevant to the process.
 */
typedef struct _pcb pcb_t;
struct _pcb
{
    uint32_t pid;
    uint32_t priority;
    int32_t sleepleft;
    fifo_queue_t inbox_q;
    process_state_t state;
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
pcb_has_pid(const void *ppcb, const void *ppid);

int
pcb_cmp_priority(const void *pcb1, const void *pcb2);

int
pcb_cmp_sleepleft(const void *pcb1, const void *pcb2);

int
pcb_is_done_sleeping(const pcb_t *pcb);

#endif
