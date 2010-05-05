#ifndef SPAWN_H
#define SPAWN_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
fib(void);

void
fib_impl(void);

int
fib_recursive(int n);

pcb_t *
spawn(user_prog_pointer program);

void
terminate_this_process(void);

void
kfunc_to_go_to_when_the_process_ends_normally(void);

#endif
