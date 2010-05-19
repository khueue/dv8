#ifndef SPAWN_H
#define SPAWN_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

pcb_t *
spawn(const char name[], user_program_pointer program, uint32_t priority);

void
kfunc_to_go_to_when_the_process_ends_normally(void);

#endif
