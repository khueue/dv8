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
#include "user_shell.h"

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

static program_t
g_program_list[3];

static void
init_program_list(void)
{
    strcpy(g_program_list[0].name, "fib");
    g_program_list[0].func = fib;

    strcpy(g_program_list[1].name, "incr");
    g_program_list[1].func = incr;

    strcpy(g_program_list[2].name, "shell");
    g_program_list[2].func = shell;
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
kexec(const char program[], uint32_t priority)
{
    pcb_t *pcb = NULL;
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        if (0 == strcmp(program, g_program_list[i].name))
        {
            pcb = spawn(g_program_list[i].func, priority);
        }
    }

    if (!pcb)
    {
        return 0;
    }

    /* Error handling here XXXXXXX */
    sch_schedule(pcb);
    return pcb->pid;
}

uint32_t
kread_message_by_type(msg_t *msg, msg_type_t type, int max_wait_ms)
{
    msg_t *msg_from_q = NULL;
    pcb_t *pcb = sch_get_currently_running_process();

    if (max_wait_ms == 0)
    {
        msg_from_q = prio_find_from_head(&pcb->inbox_q, &type);
        while (!msg_from_q)
        {
        kdebug_println("111111111 starting block now ...");
            kblock_self();
        kdebug_println("111111111 finished block now ...");
            msg_from_q = prio_find_from_head(&pcb->inbox_q, &type);
            if (msg_from_q)
            {
                
        kdebug_println("--------- hej vi har meddelande ...");
            }
        }
    }
    else
    {
        int sleepleft = max_wait_ms;
        msg_from_q = prio_find_from_head(&pcb->inbox_q, &type);
        while (!msg_from_q)
        {
            if (pcb_is_done_sleeping(pcb))
            {
                break;
            }
            ksleep(sleepleft);
            sleepleft = pcb->sleepleft;
            msg_from_q = prio_find_from_head(&pcb->inbox_q, &type);
        }
    }

    if (msg_from_q)
    {
        prio_remove_from_head(&pcb->inbox_q, &type);
        memcpy(msg, msg_from_q, sizeof(*msg));
        return 1;
    }
    else
    {
        kdebug_println("555555555 NO MESSAGE!!!!!!!!");
        return 0;
    }
}

msg_t *
read_from_console(void)
{
    pcb_t *pcb = sch_get_currently_running_process();

    tty_manager_add_input_listener(pcb);
    block_self();
    tty_manager_remove_input_listener(pcb);

    return prio_dequeue(&pcb->inbox_q);
}

/*
 * XXXXXXXXXX
 */
uint32_t
ksend_message(msg_t *msg)
{
    pcb_t *receiver = NULL;

    receiver = sch_find_process(msg_get_receiver_pid(msg));
    if (!receiver)
    {
        /* No such active process! */
        return 0;
    }

    prio_enqueue(&receiver->inbox_q, msg);
    if (0)
    {
        /* full inbox! XXXXX */
        return 0;
    }
    kunblock(receiver->pid);

    return 1;
}

uint32_t
kprint_str(const char str[])
{
    /* XXXXXXX replace with our own routine */
    kdebug_print(str);
    return 1;
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
    pcb_t *pcb = sch_get_currently_running_process();
    kkill(pcb->pid);
}

uint32_t
kkill(uint32_t pid)
{
    pcb_t *pcb = NULL;

    pcb = sch_unschedule(pid);
    pcb->state = PROCESS_STATE_TERMINATED;
    pcb = pcb_free(pcb);
    sch_run();
    return pcb == NULL; /* XXXX */
}

void
ksleep(int time)
{
    pcb_t *pcb = sch_get_currently_running_process();
    pcb->sleepleft = time * timer_msec;
    sch_sleep();
    sch_run();
}

uint32_t
kchange_priority(uint32_t pid, uint32_t priority)
{
    return sch_change_priority(pid, priority);
}

uint32_t
kblock(uint32_t pid)
{
    uint32_t r = sch_block(pid);
    sch_run();
    return r;
}

uint32_t
kunblock(uint32_t pid)
{
    uint32_t r = sch_unblock(pid);
    sch_run();
    return r;
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
    pcb_t *shell_process = NULL;
   /* pcb_t *fib_process = NULL; */

    sch_init();

    idle_process = spawn(idle, 0);
    sch_schedule(idle_process);

    /* fib_process = spawn(fib, PROCESS_DEFAULT_PRIORITY);
    sch_schedule(fib_process);*/

    shell_process = spawn(shell, PROCESS_DEFAULT_PRIORITY);
    sch_schedule(shell_process);

    sch_schedule(spawn(maltascr, PROCESS_DEFAULT_PRIORITY)); /* remove XXXXX */

    /* Initialise timer to interrupt soon. */
    kload_timer(MS_TO_NEXT_TIMER_INTERRUPT * timer_msec);
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

    init_program_list();

    tty_manager_init();

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
