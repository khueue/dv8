#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "utils.h"
#include "syscall.h"

#include "exception_syscall.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Handles syscall exception.
 */
void
execute_syscall(cause_reg_t cause)
{
    /* Make sure that we are here because of a syscall exception. */
    if (cause.field.exc == BIT3)
    {
        /* Get pointer to stored registers. */
        registers_t* regs = kget_registers();

        /* Handle the system call (see syscall.S). */
        ksyscall_handler(regs);

        /* Return from exception to instruction following syscall. */
        regs->epc_reg += 4;

        /* Acknowledge syscall exception. */
        kset_cause(~0x60, 0);
    }
}
