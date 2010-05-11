#include "utils.h"
#include "asm.h"
#include "malta.h"
#include "kernel_api.h"

#include "kernel.h"
#include "exception_uart.h"
#include "exception_timer.h"
#include "exception_syscall.h"
#include "pcb.h"
#include "spawn.h"
#include "scheduler.h"
#include "tty_manager.h"
#include "msg.h"

#include "user_fib.h"
#include "user_incr.h"
#include "user_idle.h"
#include "user_scroller.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * Registers used by the exception handler.
 */
static registers_t
g_excn_regs;

/*
 * ---------------------------------------------------------------------------
 * Private functions.
 * ---------------------------------------------------------------------------
 */

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Must be called from within an exception. Saves the state of the CPU (the
 * state just before the exception occurred) into the given process.
 */
void
save_process_state(pcb_t *pcb)
{
    memcpy(&pcb->regs, kget_registers(), sizeof(pcb->regs));
}

/*
 * Must be called from within an exception. When the exception finishes, the
 * CPU is loaded with the state of the given process.
 */
void
restore_process_state(const pcb_t *pcb)
{
    kset_registers(&pcb->regs);
}

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

uint32_t
kexec(user_program_pointer program, uint32_t priority)
{
    pcb_t *pcb = spawn(program, priority);
    /* Error handling here XXXXXXX */
    sch_schedule(pcb);
    return pcb->pid;
}

/*
 * XXXXXXX remove etc
 */
msg_t *
read_inbox_message(void)
{
    pcb_t *pcb = sch_get_currently_running_process();
    while (pcb->inbox_q.length == 0)
    {
    }
    return fifo_dequeue(&pcb->inbox_q);
}

uint32_t
kgetpid(void)
{
    pcb_t *pcb = sch_get_currently_running_process();
    return pcb->pid;
}

uint32_t
getpid(void)
{
    return do_syscall(kgetpid);
}

void
kkill_self(void)
{
    pcb_t* pcb = sch_get_currently_running_process();
    sch_remove_from_run(pcb);
    pcb->state = PROCESS_STATE_TERMINATED;
    pcb = pcb_free(pcb);
    sch_run();
}

void
ksleep(int time)
{
    pcb_t* pcb = sch_get_currently_running_process();
    pcb->sleepleft = time * timer_msec;
    sch_sleep();
    sch_run();
}

uint32_t
kchange_priority(uint32_t pid, uint32_t priority)
{
    return sch_change_priority(pid, priority);
}

void
kblock(uint32_t pid)
{
    sch_block(pid);
    sch_run();
}

void
kunblock(uint32_t pid)
{
    sch_unblock(pid);
    sch_run();
}

void
kblock_self()
{
    uint32_t pid = sch_get_currently_running_process()->pid;
    kblock(pid);
}

/*
 * Sets up the
 */
static void
setup_scheduler(void)
{
    pcb_t *idle_process = NULL;
    pcb_t *fib_process = NULL;
    
    sch_init();

    idle_process = spawn(idle, 0);
    sch_schedule(idle_process);
    fib_process = spawn(fib, PROCESS_DEFAULT_PRIORITY);
    sch_schedule(fib_process);  /* remove XXXXX */
    tty_manager_register_for_input(fib_process);
    sch_schedule(spawn(incr, PROCESS_DEFAULT_PRIORITY)); /* remove XXXXX */
    sch_schedule(spawn(maltascr, PROCESS_DEFAULT_PRIORITY)); /* remove XXXXX */
    
    /* Initialise timer to interrupt in 50 ms (simulated time). */
    kload_timer(50 * timer_msec);
}

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
    kset_registers(&g_excn_regs);

    /* Setup status register in the CPU. */
    set_status_reg();

    setup_scheduler();

    /* XXXXXXXX run scheduler? start shell? */
    do_nothing_forever("Kernel doing nothing ...", 10000000);
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
