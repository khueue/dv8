#include "utils.h"
#include "asm.h"
#include "malta.h"
#include "debug.h"

#include "uart.h"
#include "timer.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXXXXX
 */
static registers_t
regs;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

#if 0
static void
block_until_tty_ready(void)
{
    while (!tty->lsr.field.thre)
    {
        /* Block forever. */
    }
}

/* example - function that prints a string to the terminal window  */
static void
printstr(const char str[])
{
    size_t i = 0;
    while (str[i])
    {
        block_until_tty_ready();
        tty->thr = str[i];

        if (str[i]=='\n')
        {
            block_until_tty_ready();
            tty->thr = '\r';
        }

        ++i;
    }
}
#endif

/*
 * Displays a value on the Malta display.
 */
void
display_word(uint32_t word);
void
display_word(uint32_t word)
{
    int i = 0;
    malta->ledbar.reg = 0xFF;
    for (i = 7; i >= 0; --i)
    {
        malta->asciipos[i].value = '0' + (word % 10);
        word /= 10;
    }
}

/* Declaration of system call handler (defined in 'syscall.S'). */
void
ksyscall_handler(registers_t* reg);

/* Declaration of my system call for user processes. */
void
print_int(uint32_t v);

/* Kernels internal definition of my system call (prefix 'k'). */
void
kprint_int(uint32_t v);
void
kprint_int(uint32_t v)
{
    /* Implementation of my_system_call: */
    /* Displays value of its argument.   */
    display_word(v);
}

static void
execute_syscall(cause_reg_t cause)
{
    /* Make sure that we are here because of a syscall exception. */
    if (cause.field.exc == BIT3)
    {
        /* Get pointer to stored registers. */
        registers_t* reg = kget_registers();

        /* Handle the system call (see syscall.S). */
        ksyscall_handler(reg);

        /* Return from exception to instruction following syscall. */
        reg->epc_reg += 4;

        /* Acknowledge syscall exception. */
        kset_cause(~0x60, 0);
    }
}

/*
 * XXXXXXX todo
 */
static void
set_status_reg(void)
{
    status_reg_t and;
    status_reg_t or;

    /*
     * Below is an alternative way of setting the status register, using the
     * 'status_reg_t' type defined in 'mips4kc.h'.
     */
    and.reg = 0xFFFFFFFF;
    and.field.exl = 0;     /* Normal level (not exception). */
    and.field.erl = 0;     /* Error level. */
    and.field.um  = 0;     /* Kernel mode. */
    and.field.im  = BIT7|BIT2;     /* Disable all except HW interrupt 0. */
    and.field.bev = 0;     /* Use normal exception vector (not bootstrap). */

    or.reg = 0;
    or.field.ie   = 1;     /* Enable interrupts. */
    or.field.im   = BIT7|BIT2;     /*XXXX Enable HW interrupt 0. */
    or.field.cu0  = 1;     /* Coprocessor 0 usable. */

    kset_sr(and.reg, or.reg);
}

/*
 * XXXXXXXX
 */
void
kinit(void)
{
    int i = 0;
    /*
     * Set UART word length ('3' meaning 8 bits).
     * Do this early to enable debug printouts (e.g. kdebug_print).
     */
    tty->lcr.field.wls = BIT1|BIT0;

    /* Generate interrupts when data is received by UART. */
    tty->ier.field.erbfi = 1;

    /* Some obscure bit that needs to be set for UART interrupts to work. */
    tty->mcr.field.out2 = 1;

    /* Setup storage-area for saving registers on exception. */
    kset_registers(&regs);

    /* XXXXXXXXXXXX todo */
    set_status_reg();

    /* Display 0x42 using my system call */
    print_int(0x42);

    /* Initialise timer to interrupt in 100 ms (simulated time). */
    kload_timer(100 * timer_msec);

    /* run scheduler? start shell? */
    while (1)
    {
    }
}

/*
 * XXXXXXXX
 */
void
kexception(void)
{
    cause_reg_t cause;

    cause.reg = kget_cause();
    if (cause.field.ip & BIT7)
    {
        execute_timer(cause);
    }
    else if (cause.field.exc == 0)
    {
        execute_uart(cause);
    }
    else if (cause.field.exc == 8)
    {
        execute_syscall(cause);
    }
}
