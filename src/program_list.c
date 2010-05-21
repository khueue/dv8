#include "utils.h"
#include "program_list.h"

#include "user_fib.h"
#include "user_incr.h"
#include "user_idle.h"
#include "user_scroller.h"
#include "user_shell.h"
#include "user_ring.h"
#include "user_scrollermsg.h"
#include "user_philo.h"
#include "user_supervisor_demo.h"
#include "user_spammer.h"
#include "user_dv8.h"
#include "user_man.h"

#include "kernel_api.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * Maximum length of a program name.
 */
#define PROGRAM_NAME_LENGTH 64

/*
 * Used as flags for the executable_by_shell attribute in the program info
 * struct.
 */
#define SHELL     1
#define NOT_SHELL 0

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

struct program_info_
{
    char name[PROGRAM_NAME_LENGTH]; /* Name of program. */
    user_program_pointer code;      /* Code entry point. */
    int executable_by_shell;        /* Whether shell may start it. */
};

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * Table of all available programs.
 */
static const program_info_t
g_programs[] =
{
    /* name string          code pointer       shell? */
    {  "idle",              idle,              NOT_SHELL  },
    {  "fib",               fib,               SHELL      },
    {  "incr",              incr,              SHELL      },
    {  "shell",             shell,             SHELL      },
    {  "ring",              ring,              SHELL      },
    {  "ringn",             ringnode,          NOT_SHELL  },
    {  "scroll",            scroll,            NOT_SHELL  },
    {  "scrollmsg",         scrollermsg,       SHELL      },
    {  "dp",                dphilo_init,       SHELL      },
    {  "dphilo",            philosopher,       NOT_SHELL  },
    {  "dtable",            dtable,            NOT_SHELL  },
    {  "superd",            supervisor_demo,   SHELL      },
    {  "spammer",           spammer,           NOT_SHELL  },
    {  "dv8",               dv8,               NOT_SHELL  },
    {  "man",               man,               NOT_SHELL  },
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

user_program_pointer
program_list_get_program_code(const char program_name[])
{
    size_t i = 0;

    for (i = 0; i < COUNT_ARRAY(g_programs); ++i)
    {
        if (0 == strcmp(g_programs[i].name, program_name))
        {
            return g_programs[i].code;
        }
    }

    return NULL;
}

void
program_list_print(void)
{
    size_t i = 0;

    for (i = 0; i < COUNT_ARRAY(g_programs); ++i)
    {
        if (program_list_program_executable_by_shell(&g_programs[i]))
        {
            print_strln(g_programs[i].name);
        }
    }
}

int
program_list_program_executable_by_shell(const program_info_t *program)
{
    return program->executable_by_shell == SHELL;
}
