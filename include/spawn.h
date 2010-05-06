#ifndef SPAWN_H
#define SPAWN_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

pcb_t *
spawn(user_prog_pointer program);

void
terminate_this_process(void);

void
kfunc_to_go_to_when_the_process_ends_normally(void);

#endif
