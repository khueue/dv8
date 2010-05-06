#include "utils.h"
#include "asm.h"
#include "malta.h"
#include "debug.h"

#include "pcb.h"
#include "spawn.h"

extern pcb_t *p1, *p2;

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
fib_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    int n = 5;
    /* TODO: Print to tty.
    printf("fib(5) = %d\n", fib_recursive(n)); */
    kdebug_print("fib_recursive(5) = ");
    kdebug_printint(fib_recursive(n));
    kdebug_println("");
}

void
inc_impl(void)
{
    /* TODO: We should actually read arguments from messages. */
    static int i = 0;
    /* TODO: Print to tty.
    printf("fib(5) = %d\n", fib_recursive(n)); */
    kdebug_print("i = ");
    kdebug_printint(i++);
    kdebug_println("");
}

void
terminate_this_process(void)
{
    /* What does this do? */
    kdebug_println("terminating process");
}

void
fib(void)
{
    kdebug_println("start of fib");
    fib_impl();
    terminate_this_process();
    kdebug_println("end of fib");

    while (1)
    {
        static int count = 0;
        if ((count++ % 10000000) == 0)
        {
            kdebug_println("- fib whiling ...");
        }
        /*if((k++ % 10000)==0) {
            kdebug_printint(k);
            kdebug_println("fib");
        }*/
    }
    /* We should never get here! */
}

void
inc(void)
{
    kdebug_println("start of incrment");
    inc_impl();
    terminate_this_process();
    kdebug_println("end of innremtmnt");
    while (1)
    {
        static int count = 0;
        if ((count++ % 10000000) == 0)
        {
            kdebug_println("--- inc whiling ...");
        }
        /*if((k++ % 10000)==0) {
            kdebug_printint(k);
            kdebug_println("inc");
        }*/
    }
    /* We should never get here! */
}

void
maltascr(void)
{
    kdebug_println("start of maltascr");

    int strpos = 0;
    int init = 0;

    /* Funkar för alla strängar utom något som BÖRJAR med null */

    uint8_t tomalta[11];
    tomalta[0] = 'b';
    tomalta[1] = 'a';
    tomalta[2] = 'l';
    tomalta[3] = 'l';
    tomalta[4] = 'e';
    tomalta[5] = 'f';
    tomalta[6] = 'j';
    tomalta[7] = 'o';
    tomalta[8] = 'n';
    tomalta[9] = 'g';
    tomalta[10] = 0;

    malta->ledbar.reg = 0xFF;

	int done;

    while(1) {
    	static int count = 0;
        if ((count++ % 5000000) == 0)
        {
            kdebug_println("- maltascr whiling ...");
            
            if((init > 0) && (tomalta[init-1] == 0))
            {
                init = 0;
            }
            
            strpos = init++;

            for(done = 0; done < 8; done++) 
            {            
                if(tomalta[strpos] == 0)
                {
                    strpos = 0;
                    malta->asciipos[done++].value = 32;
                }

                if(done < 8) {
                    malta->asciipos[done].value = tomalta[strpos++];
                }                    
            }
            
        }

        /* Done printing to malta, jump back to scheduler ...somehow*/
    }
}

#if 0
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
#endif

void
kfunc_to_go_to_when_the_process_ends_normally(void)
{
    kdebug_println("process ended normally, whiling ...");

    {
        static int i = 0;
        if (i == 0)
        {
            i = 1;
            registers_t regs;
            pcb_t *process = spawn(fib);
            kswitch_context(&process->regs, &regs);
            while (1)
            {
            }
        }
    }
    while (1)
    {
    }
}

pcb_t *
spawn(user_prog_pointer program)
{
    pcb_t *pcb = pcb_alloc();

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
