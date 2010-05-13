#include "utils.h"
#include "kernel.h"
#include "kernel_api.h"

#include "user_shell.h"
#include "user_fib.h"
#include "user_incr.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXX
 */
static char
g_line[1024];

/*
 * XXXXXXXX
 */
static char *
g_args[512];

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXX
 */
static char *
skipwhite(char str[])
{
    while (*str == ' ')
    {
        ++str;
    }
    return str;
}

/*
 * XXXXX
 */
static void
split(char cmd[])
{
    cmd = skipwhite(cmd);
    char *next = strchr(cmd, ' ');
    size_t i = 0;

    while (next)
    {
        *next = '\0';
        g_args[i] = cmd;
        ++i;
        cmd = skipwhite(next + 1);
        next = strchr(cmd, ' ');
    }

    if (*cmd)
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
    if (exec(g_args[0], PROCESS_DEFAULT_PRIORITY))
    {
        kdebug_print("Executed ");
        kdebug_println(g_args[0]);
    }
    else
    {
        kdebug_print("Unknown command ");
        kdebug_println(g_args[0]);
    }

    return 1;
}

/*
 * XXXXX
 */
static int
run(char cmd[])
{
    split(cmd);
    if (g_args[0])
    {
        if (0 == strcmp(g_args[0], "exit"))
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
    kdebug_println("SHELL: type 'exit' to exit.");
    while (1)
    {
        msg_t *msg = NULL;

        kdebug_print("deviate> ");
        msg = read_from_console();
        strcpy(g_line, msg_data_get_string(msg));

        run(g_line);
    }
}
