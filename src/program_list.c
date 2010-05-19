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
    char name[PROGRAM_NAME_LENGTH];
    user_program_pointer code;
    int visible_from_shell;
};

#define VISIBLE     1
#define NOT_VISIBLE 0

static program_info_t
g_programs[] =
{
    {  "idle",              idle,              NOT_VISIBLE  },
    {  "fib",               fib,               VISIBLE      },
    {  "incr",              incr,              VISIBLE      },
    {  "shell",             shell,             VISIBLE      },
    {  "ring",              ring,              VISIBLE      },
    {  "ringn",             ringnode,          NOT_VISIBLE  },
    {  "scroll",            scroll,            NOT_VISIBLE  },
    {  "scrollermsg",       scrollermsg,       VISIBLE      },
    {  "dp",                dphilo_init,       VISIBLE      },
    {  "philo",             philosopher,       NOT_VISIBLE  },
    {  "supervisor_demo",   supervisor_demo,   VISIBLE      },
    {  "spammer",           spammer,           NOT_VISIBLE  },
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
