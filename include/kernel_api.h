#ifndef KERNEL_API_H
#define KERNEL_API_H

#include "utils.h"
#include "registers.h"
#include "msg.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
kill_self(void);

uint32_t
kill(uint32_t pid);

uint32_t
do_syscall(uint32_t (*kfunc)(void));

uint32_t
ksyscall_handler(registers_t *regs);

uint32_t
exec(const char *program, uint32_t priority);

void
sleep(int time);

uint32_t
change_priority(uint32_t pid, uint32_t priority);

void
print_char(char c);

void
print_str(const char str[]);

void
print_strln(const char str[]);

void
print_int(int x);

void
top(void);

uint32_t
send_message(msg_t *msg);

uint32_t
read_from_console(msg_t *msg);

uint32_t
read_message_by_type(msg_t *msg, msg_type_t type, int max_wait_ms);

uint32_t
read_next_message(msg_t *msg, int max_wait_ms);

uint32_t
block(uint32_t pid);

uint32_t
unblock(uint32_t pid);

void
block_self(void);

void
supervise(uint32_t pid);

#endif
