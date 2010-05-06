#include <mips/regdef.h>
#include <cp0_regdef.h>
#include "asm_regs.h"

    .text
    .globl ksyscall_handler
    .globl exec

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

    # Input:XXXXXX
exec:
    la v0, kexec
    syscall
    jr ra