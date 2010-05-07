#ifndef KERNEL_API_H
#define KERNEL_API_H

#include "utils.h"
#include "registers.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/* kill */
void
kill_self(void);

void
ksyscall_handler(registers_t* regs);

uint32_t
exec(user_prog_pointer program);

#endif
