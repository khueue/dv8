#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "utils.h"
#include "pcb.h"

void
sch_init(void);

void
sch_decrease_sleep(void);

void
sch_run(void);

void
sch_sleep(void);

void
sch_schedule(pcb_t *pcb);

pcb_t *
sch_unschedule(uint32_t pid);

void
sch_remove_from_run(pcb_t *pcb);

pcb_t *
sch_find_process(uint32_t pid);

pcb_t *
sch_get_currently_running_process(void);

uint32_t
sch_change_priority(uint32_t pid, uint32_t prio);

uint32_t
sch_block(uint32_t pid);

uint32_t
sch_unblock(uint32_t pid);

void
    sch_print(void);
#endif
