#include "utils.h"
#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "syscall.h"

#include "exception_uart.h"
#include "exception_timer.h"
#include "exception_syscall.h"
#include "pcb.h" /* XXX probably remove */
#include "spawn.h" /* XXX probably remove */

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

#if 0
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
#endif

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
    //or.field.ie  = 0;         /* Enable interrupts. */
    or.field.im  = BIT7|BIT2; /* XXXXXX todo: Enable HW interrupt 0. */
    //or.field.im  = 0; /* XXXXXX todo: Enable HW interrupt 0. */
    or.field.cu0 = 1;         /* Coprocessor 0 usable. */

    kset_sr(and.reg, or.reg);
}

pcb_t *p1, *p2;

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



    {
        p1 = spawn(fib);
        p2 = spawn(inc);
    }
    /* Initialise timer to interrupt in 50 ms (simulated time). */
    kload_timer(50 * timer_msec);
    /* XXXXXXXX run scheduler? start shell? */
    kdebug_println("KERNEL whiling ...");
    while (1)
    {
        static int count = 0;
        if ((count++ % 10000000) == 0)
        {
            kdebug_println("Kernel whiling ...");
        }
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
    else
    {
        if (cause.field.exc == 3)
        {
            kdebug_println("############ cause exc 3, TLB store ############");
        }
        else if (cause.field.exc == 4)
        {
            kdebug_println("############ address error exc load/fetch ############");
        }
        else if (cause.field.exc == 5)
        {
            kdebug_println("############ address error exc store ############");
        }
        else
        {
            kdebug_println("############ NU BLEV DET SKIT med something ############");
        }

        while (1)
        {
        }
    }
}
