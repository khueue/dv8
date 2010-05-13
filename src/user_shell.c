#include "utils.h"
#include "kernel.h"
#include "kernel_api.h"

#include "user_shell.h"
#include "user_fib.h"
#include "user_incr.h"

static char
g_line[1024];

static char *
g_args[512];

/*
 * XXXXX
 */
static char *
skipwhite(char* s)
{
    while (*s == ' ')
    {
        ++s;
    }
    return s;
}

/*
 * XXXXX
 */
static void
split(char *cmd)
{
    cmd = skipwhite(cmd);
    char *next = strchr(cmd, ' ');
    int i = 0;
    while (next != NULL)
    {
        *next = '\0';
        g_args[i] = cmd;
        ++i;
        cmd = skipwhite(next + 1);
        next = strchr(cmd, ' ');

    }

    if (*cmd != '\0')
    {
        g_args[i] = cmd;
        next = strchr(cmd, '\0');
        *next = '\0';
        ++i;
    }

    g_args[i] = NULL;
}

/*
 * XXXXX
 */
static int
command(void)
{
    if(exec(g_args[0], PROCESS_DEFAULT_PRIORITY)) 
    {
        kdebug_print("Executed ");
        kdebug_println(g_args[0]);
    }
    else
    {    
        kdebug_print("Unknow command ");
        kdebug_println(g_args[0]);    
    }            
    return 1;
}

/*
 * XXXXX
 */
static int
run(char* cmd)
{
    split(cmd);
    if (g_args[0] != NULL)
    {
        if (strcmp(g_args[0], "exit") == 0)
        {
            kill_self();
        }
        return command();
    }
    return 0;
}

/*
 * XXXXX
 */
void
shell(void)
{
    kdebug_println("SIMPLE SHELL: Type 'exit' to exit.");
    while (1)
    {
        msg_t *msg = NULL;

        /* Print the command prompt */
        kdebug_print("$> ");
        msg = read_from_console();
        if (msg_data_is_string(msg))
        {
            strcpy(g_line, msg_data_get_string(msg));
        }

        run(g_line);
    }
}
