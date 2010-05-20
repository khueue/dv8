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

#include "program_list.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Must be called from within an exception. When the exception finishes, the
 * CPU is loaded with the state of the given process.
 */
void
restore_process_state(const pcb_t *pcb)
{
    kset_registers(&pcb->regs);
}

uint32_t
kexec(const char program[], uint32_t priority)
{
    user_program_pointer code = NULL;
    pcb_t *pcb = NULL;

    code = program_list_get_program_code(program);
    if (!code)
    {
        /* xxxxxxxx error invalid program */
        return 0;
    }

    pcb = spawn(program, code, priority);
    if (!pcb)
    {
        /* crappy spawn xxxxxxx */
        return 0;
    }

    sch_schedule(pcb);

    return pcb->pid;
}

uint32_t
kread_message_by_type(msg_t *msg, msg_type_t type, int max_wait_ms)
{
    pcb_t *pcb = sch_get_currently_running_process();
    msg_t *msg_from_q = NULL;

    msg_zero(msg);

    msg_from_q = prio_find_from_head(&pcb->inbox_q, &type);
    if (msg_from_q)
    {
        prio_remove_from_head(&pcb->inbox_q, &type);
        msg_copy(msg, msg_from_q);
        msg_from_q = msg_free(msg_from_q);
    }
    else
    {
        pcb->waiting_msg = msg;
        pcb->waiting_type = type;
        if (max_wait_ms == 0)
        {
            kblock_self();
        }
        else
        {
            ksleep(max_wait_ms);
        }
    }

    return 1;
}

uint32_t
kread_from_console(msg_t *msg)
{
    pcb_t *pcb = sch_get_currently_running_process();
    msg_t *msg_from_q = NULL;
    msg_type_t type = MSG_TYPE_CONSOLE_INPUT;

    msg_zero(msg);

    /* Remove any pending console input instead of dealing with it. */
    while ((msg_from_q = prio_find(&pcb->inbox_q, &type)))
    {
        msg_free(msg_from_q);
        prio_remove(&pcb->inbox_q, &type);
    }

    tty_manager_add_input_listener(pcb);

    /* Guaranteed no console input in queue, so just block. */
    pcb->waiting_msg = msg;
    pcb->waiting_type = MSG_TYPE_CONSOLE_INPUT;
    kblock_self();

    return 1;
}

uint32_t
kread_next_message(msg_t *msg, int max_wait_ms)
{
    pcb_t *pcb = sch_get_currently_running_process();
    msg_t *msg_from_q = NULL;

    msg_zero(msg);

    msg_from_q = prio_find_head(&pcb->inbox_q);
    if (msg_from_q)
    {
        prio_remove_head(&pcb->inbox_q);
        msg_copy(msg, msg_from_q);
        msg_from_q = msg_free(msg_from_q);
    }
    else
    {
        pcb->waiting_msg = msg;
        pcb->waiting_type = MSG_TYPE_INVALID;
        if (max_wait_ms == 0)
        {
            kblock_self();
        }
        else
        {
            ksleep(max_wait_ms);
        }
    }

    return 1;
}

/*
 * XXXXXXXXXX
 */
uint32_t
ksend_message(msg_t *msg)
{
    pcb_t *receiver = NULL;

    uint32_t sender_pid = kgetpid();

    receiver = sch_find_process(msg_get_receiver_pid(msg));
    if (!receiver)
    {
        /* No such active process! */
        return 0;
    }

    if (pcb_inbox_full(receiver))
    {
        msg = msg_free(msg);
        kkill(sender_pid);
        return 0;
    }

    if (receiver->waiting_msg != NULL && msg_type_is(msg, receiver->waiting_type))
    {
        msg_copy(receiver->waiting_msg, msg);
        msg_set_sender_pid(receiver->waiting_msg, sender_pid);
        receiver->waiting_type = MSG_TYPE_INVALID;
        receiver->waiting_msg = NULL;
        kunblock(receiver->pid);
    }
    else
    {
        msg_t *new_msg = msg_alloc();
        msg_copy(new_msg, msg);
        msg_set_sender_pid(new_msg, sender_pid);
        if (!prio_enqueue(&receiver->inbox_q, new_msg))
        {
            new_msg = msg_free(new_msg);
            return 0;
        }
    }

    return 1;
}

static void
poll_until_tty_ready(void)
{
    while (!tty->lsr.field.thre)
    {
        /* Wait for the device to become ready. */
    }
}

void
kprint_char(char c)
{
    poll_until_tty_ready();
    tty->thr = c;
}

void
kprint_str(const char str[])
{
    size_t i = 0;
    while (str[i])
    {
        kprint_char(str[i]);

        if (str[i] == '\n')
        {
            kprint_char('\r');
        }

        ++i;
    }
}

void
kprint_strln(const char str[])
{
    kprint_str(str);
    kprint_str("\n");
}

void
kprint_int(int x)
{
    /* Deal with negative numbers. */
    if (x < 0)
    {
        kprint_char('-');
        x *= -1;
    }

    /* Recursive print. */
    if (x < 10)
    {
        kprint_char('0' + x);
    }
    else
    {
        kprint_int(x / 10);
        kprint_char('0' + (x % 10));
    }
}

uint32_t
kgetpid(void)
{
    pcb_t *pcb = sch_get_currently_running_process();
    if (pcb)
    {
        return pcb->pid;
    }
    else
    {
        return 0;
    }
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
    pcb->state = PROCESS_STATE_ENDED;
    kkill(pcb->pid);
}

uint32_t
kkill(uint32_t pid)
{
    pcb_t *pcb = NULL;

    pcb = sch_unschedule(pid);

    if (!pcb)
    {
        /* No process found with given pid! */
        return 0;
    }

    if (pcb->state != PROCESS_STATE_ENDED)
    {
        pcb->state = PROCESS_STATE_TERMINATED;
    }

    if (pcb->supervisor_pid)
    {
        msg_t *msg = msg_alloc();
        msg_set_receiver_pid(msg, pcb->supervisor_pid);
        msg_set_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_ID);
        msg_data_set_integer(msg, pcb->pid);
        ksend_message(msg);

        msg_set_type(msg, MSG_TYPE_SUPERVISOR_NOTICE_STATE);
        msg_data_set_integer(msg, pcb->state);
        ksend_message(msg);

        msg = msg_free(msg);
    }

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

uint32_t
kblock_self(void)
{
    pcb_t *process = sch_get_currently_running_process();
    if (process)
    {
        kblock(process->pid);
        return 1;
    }
    else
    {
        return 0;
    }
}

void
ksupervise(uint32_t pid)
{
    pcb_t *process = sch_find_process(pid);
    int supervisor = kgetpid();
    pcb_assign_supervisor(process, supervisor);
}

void
kset_inbox_limit(uint32_t limit)
{
    pcb_t *process = sch_get_currently_running_process();
    kdebug_assert(process);
    process->inbox_limit = limit;
}

/*
 * Sets up the
 */
static void
setup_scheduler(void)
{
    uint32_t ret = 0;

    sch_init();

    ret = kexec("idle", 0);
    if (!ret)
    {
        kdebug_println("Failed to start the idle process!");
        return;
    }

    ret = kexec("scroll", PROCESS_DEFAULT_PRIORITY + 30);
    if (!ret)
    {
        kdebug_println("Failed to start the scroller!");
        return;
    }

    ret = kexec("shell", PROCESS_DEFAULT_PRIORITY);
    if (!ret)
    {
        kdebug_println("Failed to start the shell!");
        return;
    }

    /* Initialize timer to interrupt soon. */
    kload_timer(MS_TO_NEXT_TIMER_INTERRUPT * timer_msec);
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

/*
 * Entry point for the C code. We start here when the assembly has finished
 * some initial work.
 */
void
kinit(void)
{
    /* Registers used by the exception handler. */
    registers_t excn_regs;

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
    kset_registers(&excn_regs);

    /* Setup status register in the CPU. */
    set_status_reg();

    tty_manager_init();

    setup_scheduler();

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
            kdebug_println("!!! CAUSE EXC 3, TLB STORE !!!");
        }
        else if (cause.field.exc == 4)
        {
            kdebug_println("!!! ADDRESS ERROR EXC LOAD/FETCH !!!");
        }
        else if (cause.field.exc == 5)
        {
            kdebug_println("!!! ADDRESS ERROR EXC STORE !!!");
        }
        else
        {
            kdebug_println("!!! SOMETHING BAD HAPPENED !!!");
        }

        while (1)
        {
        }
    }
}
