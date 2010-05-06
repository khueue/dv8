#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "utils.h"
#include "asm.h"
#include "debug.h"

#include "pcb.h"

void
sch_init(void);

pcb_t *
sch_run(void);

void
sch_schedule(pcb_t *pcb);

void
sch_place_in_run(pcb_t *pcb);

pcb_t *
sch_get_current_running(void);

#endif
