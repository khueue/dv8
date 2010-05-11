#ifndef KERNEL_API_H
#define KERNEL_API_H

#include "utils.h"
#include "registers.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
kill_self(void);

uint32_t
do_syscall(uint32_t (*kfunc)(void));

uint32_t
ksyscall_handler(registers_t *regs);

uint32_t
exec(user_program_pointer program);

void
sleep(int time);

uint32_t
change_priority(uint32_t pid, uint32_t priority);

uint32_t
block(uint32_t pid);

uint32_t
unblock(uint32_t pid);

void
block_self(void);

#endif
