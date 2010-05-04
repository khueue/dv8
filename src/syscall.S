#include <mips/regdef.h>
#include <cp0_regdef.h>
#include "asm_regs.h"

    .text
    .globl print_int
    .globl kprint_int
    .globl ksyscall_handler
    .globl switch_to_registers

    # Input:
    #   a0 - registers_t *
switch_to_registers:
    addi v0, a0, 0
    syscall
    jr ra

    # A:
    #   A user mode interface to the kernel mode function
    #   'kmy_system_call'.
    #
print_int:
    la v0, kprint_int   # Load adress of syscall implementation in v0.
    syscall         # Enter kernel mode by 'syscall' exception.
    jr ra           # Back in user mode, return to caller

ksyscall_handler:
    lw v0, REG_V0(a0)
    lw v0, REG_EPC(v0)
    jr v0

    # syscall_handler:
    #   Enables argument passing for up
    #   to four parameters of a system call.
    #
    # Input:
    #   a0 - Pointer to saved registers.
    #
