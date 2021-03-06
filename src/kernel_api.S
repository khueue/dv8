#include <mips/regdef.h>
#include <cp0_regdef.h>
#include "asm_regs.h"

    .text
    .globl ksyscall_handler
    .globl exec
    .globl do_syscall
    .globl kill_self
    .globl sleep
    .globl change_priority
    .globl block
    .globl unblock
    .globl block_self
    .globl kill
    .globl send_message
    .globl print_char
    .globl print_str
    .globl print_strln
    .globl print_int
    .globl read_message_by_type
    .globl read_next_message
    .globl read_from_console
    .globl top
    .globl supervise
    .globl set_inbox_limit
    .globl get_process_name
    .globl get_process_priority
    .globl get_process_state
    .globl lcd_print
    .globl getpid

    # syscall_handler:
    #   Enables argument passing for up
    #   to four parameters of a system call.
    #
    # Input:
    #   a0 - Pointer to saved registers.
    #
ksyscall_handler:
    addi    v0, a0, 0   # Move a0 into v0.
    lw  a0, REG_A0(v0)  # Load saved a0 (argument 1) into a0.
    lw  a1, REG_A1(v0)  # Load saved a1 (argument 2) into a1.
    lw  a2, REG_A2(v0)  # Load saved a2 (argument 3) into a2.
    lw  a3, REG_A3(v0)  # Load saved a3 (argument 4) into a3.
    lw  v0, REG_V0(v0)  # Load saved v0 (implementation) into v0.

    jr  v0      # Jump to system call implementation.

exec:
    la v0, kexec
    syscall
    jr ra

do_syscall:
    addi v0, a0, 0
    syscall
    jr ra

kill_self:
    la v0, kkill_self
    syscall
    jr ra

kill:
    la v0, kkill
    syscall
    jr ra

sleep:
    la v0, ksleep
    syscall
    jr ra

change_priority:
    la v0, kchange_priority
    syscall
    jr ra

block:
    la v0, kblock
    syscall
    jr ra

unblock:
    la v0, kunblock
    syscall
    jr ra

block_self:
    la v0, kblock_self
    syscall
    jr ra

print_char:
    la v0, kprint_char
    syscall
    jr ra

print_str:
    la v0, kprint_str
    syscall
    jr ra

print_strln:
    la v0, kprint_strln
    syscall
    jr ra

print_int:
    la v0, kprint_int
    syscall
    jr ra

send_message:
    la v0, ksend_message
    syscall
    jr ra

read_message_by_type:
    la v0, kread_message_by_type
    syscall
    jr ra

read_next_message:
    la v0, kread_next_message
    syscall
    jr ra

top:
    la v0, sch_print
    syscall
    jr ra

get_process_name:
    la v0, kget_process_name
    syscall
    jr ra

get_process_priority:
    la v0, kget_process_priority
    syscall
    jr ra

get_process_state:
    la v0, kget_process_state
    syscall
    jr ra

supervise:
    la v0, ksupervise
    syscall
    jr ra

set_inbox_limit:
    la v0, kset_inbox_limit
    syscall
    jr ra

read_from_console:
    la v0, kread_from_console
    syscall
    jr ra

lcd_print:
    la v0, klcd_print
    syscall
    jr ra

getpid:
    la v0, kgetpid
    syscall
    jr ra
