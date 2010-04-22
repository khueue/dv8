#include "regdef.h"
#include "bits.h"

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
    # Set the UART word length to 8 bits.
    li  t0, 0xb80003f8  # UART Base address
    lb  t1, 0x03(t0)    # Line Status register

    # Set word length to 8 bits (two least significant bits to 1), see TTY manual, page 14 and 15.
    ori t1, t1, BIT0|BIT1
    sb  t1, 0x03(t0)

    la  sp, kernel_boot_stack_high_limit
    j   kinit
