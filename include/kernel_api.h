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
exec(user_prog_pointer program);

void
sleep(int time);

uint32_t
change_priority(uint32_t pid, uint32_t priority);

void
block(uint32_t pid);

void
unblock(uint32_t pid);

void
block_self(void);

#endif
