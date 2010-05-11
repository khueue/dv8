#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "utils.h"
#include "pcb.h"

void
sch_init(void);

void
sch_run(void);

void
sch_sleep(void);

void
sch_schedule(pcb_t *pcb);

void
sch_remove_from_run(pcb_t *pcb);

pcb_t *
sch_get_currently_running_process(void);

void
sch_change_priority(uint32_t pid, uint32_t prio);

void
sch_block(uint32_t pid);

void
sch_unblock(uint32_t pid);

#endif
