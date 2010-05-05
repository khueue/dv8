#ifndef SYSCALL_H
#define SYSCALL_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
ksyscall_handler(registers_t* regs);

void
switch_to_registers(registers_t *regs);

#endif
