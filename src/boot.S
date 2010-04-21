#include "regdef.h"

    .data

# -----------------------------------------------------
# Stack.
# -----------------------------------------------------
kernel_boot_stack_low_limit:
    .align  2
    #define KERNEL_BOOT_STACK_SIZE  0x4000
    .space  KERNEL_BOOT_STACK_SIZE
kernel_boot_stack_high_limit:

    .text
    .set    nomove

    .globl  _start

# -----------------------------------------------------
# Program entry point. Initiates the kernel.
# -----------------------------------------------------
_start:
    la  sp, kernel_boot_stack_high_limit
    j   kinit
