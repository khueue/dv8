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
g_regs;

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
ksyscall_handler(registers_t* regs);

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

/*
 * Handles syscall exceptions.
 */
static void
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

/*
 * Configures the CPU to enable interrupts etc.
 */
static void
set_status_reg(void)
{
    status_reg_t and;
    status_reg_t or;

    /*
     * Set the status register in the CPU by turning off bits with 'and' and
     * turning on bits with 'or'.
     */
    and.reg = 0xFFFFFFFF; /* All ones: preserve all bits. */
    and.field.exl = 0;    /* Normal level (not exception). */
    and.field.erl = 0;    /* Error level. */
    and.field.um  = 0;    /* Kernel mode. */
    and.field.bev = 0;    /* Use normal exception vector (not bootstrap). */

    or.reg = 0;               /* All zeroes: preserve all bits. */
    or.field.ie  = 1;         /* Enable interrupts. */
    or.field.im  = BIT7|BIT2; /* XXXXXX todo: Enable HW interrupt 0. */
    or.field.cu0 = 1;         /* Coprocessor 0 usable. */

    kset_sr(and.reg, or.reg);
}

int
fib_recursive(int n);
void
fib(void);
#include "pcb.h"
pcb_t *
spawn(user_prog_pointer program);

void switch_to_registers(registers_t *regs);

/*
 * Entry point for the C code. We start here when the assembly has finished
 * some initial work.
 */
void
kinit(void)
{
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
    kset_registers(&g_regs);

    /* Setup status register in the CPU. */
    set_status_reg();

    /* Initialise timer to interrupt in 50 ms (simulated time). */
    kload_timer(50 * timer_msec);

    {
        pcb_t *process = spawn(fib);
        switch_to_registers(&process->regs);
    }
    
    /* XXXXXXXX run scheduler? start shell? */
    kdebug_println("KERNEL whiling ...");
    while (1)
    {
    }
}

/*
 * Called automatically after an exception occurs and registers are saved.
 * Handles the various exceptions. 
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
