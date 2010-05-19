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

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

#define PROGRAM_NAME_LENGTH 64

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

typedef struct program_info_ program_info_t;
struct program_info_
{
    char name[PROGRAM_NAME_LENGTH]; /* Name of program. */
    user_program_pointer code;      /* Code entry point. */
    int executable_by_shell;        /* Whether shell may start it. */
};

#define SHELL     1
#define NOT_SHELL 0

static program_info_t
g_programs[] =
{
    {  "idle",              idle,              NOT_SHELL  },
    {  "fib",               fib,               SHELL      },
    {  "incr",              incr,              SHELL      },
    {  "shell",             shell,             SHELL      },
    {  "ring",              ring,              SHELL      },
    {  "ringn",             ringnode,          NOT_SHELL  },
    {  "scroll",            scroll,            NOT_SHELL  },
    {  "scrollermsg",       scrollermsg,       SHELL      },
    {  "dp",                dphilo_init,       SHELL      },
    {  "philo",             philosopher,       NOT_SHELL  },
    {  "supervisor_demo",   supervisor_demo,   SHELL      },
    {  "spammer",           spammer,           NOT_SHELL  },
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
