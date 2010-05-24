#include "utils.h"
#include "kernel_api.h"

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

#include "program_list.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * Flags to indicate if a program is directly executable by the shell.
 */
enum
{
    SHELL,
    NOT_SHELL
};

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

struct program_info_
{
    const char *name;          /* Name of program. */
    user_program_pointer code; /* Code entry point. */
    int executable_by_shell;   /* Whether shell may start it. */
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
    /* name string          code pointer           shell?     */
    {  "idle",              idle,                  NOT_SHELL  },
    {  "fib",               fib,                   SHELL      },
    {  "incr",              incr,                  SHELL      },
    {  "shell",             shell,                 SHELL      },
    {  "ring",              ring,                  SHELL      },
    {  "ringn",             ringnode,              NOT_SHELL  },
    {  "scroll",            scroll,                NOT_SHELL  },
    {  "scrollmsg",         scrollermsg,           SHELL      },
    {  "dp",                dphilo_init,           SHELL      },
    {  "dphilo",            philosopher,           NOT_SHELL  },
    {  "dtable",            dtable,                NOT_SHELL  },
    {  "superd",            supervisor_demo,       SHELL      },
    {  "spammer",           spammer,               NOT_SHELL  },
    {  "dv8",               dv8,                   NOT_SHELL  },
    {  "man",               man,                   NOT_SHELL  },
};

/*
 * Index into program table, used by iterator.
 */
static size_t
g_iter_current_index;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
program_list_iter_reset(void)
{
    g_iter_current_index = 0;
}

int
program_list_iter_has_next(void)
{
    return g_iter_current_index < COUNT_ARRAY(g_programs);
}

const program_info_t *
program_list_iter_next(void)
{
    return &g_programs[g_iter_current_index++];
}

user_program_pointer
program_list_get_program_code(const char program_name[])
{
    program_list_iter_reset();
    while (program_list_iter_has_next())
    {
        const program_info_t *program = program_list_iter_next();
        if (0 == strcmp(program_info_get_name(program), program_name))
        {
            return program_info_get_code(program);
        }
    }

    return NULL;
}

void
program_list_print(void)
{
    program_list_iter_reset();
    while (program_list_iter_has_next())
    {
        const program_info_t *program = program_list_iter_next();
        if (program_info_is_executable_by_shell(program))
        {
            print_strln(program_info_get_name(program));
        }
    }
}

int
program_list_name_exist(char *name)
{
    size_t i = 0;

    for (i = 0; i < COUNT_ARRAY(g_programs); ++i)
    {
        if (0 == strcmp(name, g_programs[i].name))
        {
            return 1;
        }
    }
    return 0;
}

const char *
program_info_get_name(const program_info_t *program)
{
    return program->name;
}

user_program_pointer
program_info_get_code(const program_info_t *program)
{
    return program->code;
}

int
program_info_is_executable_by_shell(const program_info_t *program)
{
    return program->executable_by_shell == SHELL;
}
