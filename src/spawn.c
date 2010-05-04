#include "utils.h"
#include "asm.h"
#include "malta.h"
#include "debug.h"

#include "pcb_freelist.h"

void
print_int(uint32_t word);

int
    fib_recursive(int n);
int
fib_recursive(int n)
{
    if (n <= 1)
    {
        return 1;
    }
    else
    {
        return fib_recursive(n-1) + fib_recursive(n-2);
    }
}

void
    fib_impl(void);
void
fib_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    int n = 5;
    /* TODO: Print to tty.
    printf("fib(5) = %d\n", fib_recursive(n)); */
    print_int(fib_recursive(n));
}

void
    terminate_this_process(void);
void
terminate_this_process(void)
{
    /* What does this do? */
    kdebug_println("terminate process");
}

void
    fib(void);
void
fib(void)
{
    fib_impl();
    terminate_this_process();
    kdebug_println("end of fib");
    /* We should never get here! */
}

enum
{
    USER_PROG_FIB = 0,
};

user_prog_pointer
g_program_list[] =
{
    /* 0 */ fib,
    NULL,
};

void
    kfunc_to_go_to_when_the_process_ends_normally(void);
void
kfunc_to_go_to_when_the_process_ends_normally(void)
{
    kdebug_println("process ended normally, whiling ...");
    while (1)
    {
    }
}

pcb_t *
    spawn(user_prog_pointer program);
pcb_t *
spawn(user_prog_pointer program)
{
    pcb_t *pcb = alloc_pcb();

    if (!pcb)
    {
        /* XXX no more pcbs! */
        return NULL;
    }

    pcb->pid = 1;
    pcb->priority = 20;

    pcb->regs.epc_reg = (uint32_t)program;
    pcb->regs.sp_reg  = (uint32_t)(pcb->stack + sizeof(pcb->stack) - 32); /* -32 ??????????? XXX */
#define GLOBAL_DATA_OFFSET 0x80000000
    pcb->regs.gp_reg  = GLOBAL_DATA_OFFSET;
    pcb->regs.ra_reg  = (uint32_t)kfunc_to_go_to_when_the_process_ends_normally;

    return pcb;
}

#ifdef SPAWN_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char const *argv[])
{
    /* Try to run fib. */
    g_program_list[0]();

    (void)argc;
    (void)argv;
    return 0;
}

#endif
