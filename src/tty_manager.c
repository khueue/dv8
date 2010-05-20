#include "utils.h"
#include "malta.h"
#include "asm.h"
#include "tty_manager.h"
#include "stack.h"
#include "msg.h"

#include "kernel_api.h"
#include "kernel.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * A simple FIFO queue of bounded size.
 */
typedef struct bounded_fifo bounded_fifo_t;
struct bounded_fifo
{
    uint8_t buf[TTY_FIFO_SIZE];
    uint32_t pos_next_free;
};

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXX processes wanting input
 */
static stack_t
g_input_stack;

/*
 * XXXXXXXXXX
 */
static bounded_fifo_t
g_bfifo;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * bfifo_put: Inserts a character at the end of the queue.
 */
static void
bfifo_put(bounded_fifo_t *bfifo, uint8_t c)
{
    kdebug_assert(bfifo);

    if (bfifo->pos_next_free < COUNT_ARRAY(bfifo->buf))
    {
        bfifo->buf[bfifo->pos_next_free++] = c;
    }
}

/*
 * bfifo_get: Returns a character removed from the front of the queue.
 */
static uint8_t
bfifo_get(bounded_fifo_t *bfifo)
{
    size_t i = 0;
    uint8_t c = '\0';

    kdebug_assert(bfifo);
    kdebug_assert(bfifo->pos_next_free > 0);

    c = bfifo->buf[0];
    --bfifo->pos_next_free;
    for (i = 0; i < bfifo->pos_next_free; ++i)
    {
        bfifo->buf[i] = bfifo->buf[i+1];
    }

    return c;
}

void
tty_manager_init(void)
{
    stack_init(&g_input_stack, pcb_cmp_priority, pcb_has_pid);
}

/*
 * XXXXX
 */
void
tty_manager_add_input_listener(pcb_t *pcb)
{
    stack_push(&g_input_stack, pcb);
}

/*
 * XXXXX
 */
void
tty_manager_remove_input_listener(pcb_t *pcb)
{
    stack_remove(&g_input_stack, &pcb->pid);
}

/*
 * XXXXXXXX
 */
int
tty_manager_has_characters(void)
{
    return g_bfifo.pos_next_free > 0;
}

/*
 * XXXXXXXX
 */
static void
tty_manager_dispatch_message(const char str[])
{
    /* Send input message to process if we have any input listeners. */
    if (g_input_stack.length > 0)
    {
        msg_t msg_struct;
        msg_t *msg = &msg_struct;
        pcb_t *process = NULL;

        msg_type_set_console_input(msg);
        msg_data_set_string(msg, str);

        process = stack_peek(&g_input_stack);
        kdebug_assert(process);
        kdebug_println("tty::: process to receive input: ");
        kdebug_printint(process->pid);
        kdebug_println("");
        tty_manager_remove_input_listener(process);
        msg_set_receiver_pid(msg, process->pid);
        ksend_message(msg);
    }
    else
    {
        kdebug_println("TTY MANAGER: No one is waiting for input!");
    }
}

static char g_buf[STR_BUF_SIZE];
static size_t g_pos;

/*
 * Successively builds a complete line of characters from input.
 */
static void
tty_manager_build_line(uint8_t c)
{
    /* Add to buffer only if there is room. */
    if (g_pos < sizeof(g_buf))
    {
        g_buf[g_pos++] = c;
    }

    /* If the last character received was a return, proceed to finish. */
    if (c == '\r')
    {
        /* Terminate both the newline and the end of string, to be safe. */
        g_buf[g_pos-1] = '\0';
        g_buf[sizeof(g_buf)-1] = '\0';
        g_pos = 0;

        tty_manager_dispatch_message(g_buf);
    }
}

/*
 * XXXXXXXXXXX
 */
static void
tty_manager_remove_prutt(void)
{
    if (g_pos > 0)
    {
        g_buf[--g_pos] = '\0';
    }
}

/*
 * XXXXXX
 */
void
tty_manager_put_char(uint8_t c)
{
    const uint8_t BACKSPACE = 0x8;
    if (c == BACKSPACE)
    {
        if (g_pos > 0)
        {
            bfifo_put(&g_bfifo, BACKSPACE);
            bfifo_put(&g_bfifo, ' ');
            bfifo_put(&g_bfifo, BACKSPACE);
            tty_manager_remove_prutt();
        }
    }
    else
    {
        bfifo_put(&g_bfifo, c);
        if (c == '\r')
        {
            bfifo_put(&g_bfifo, '\n');
        }
        tty_manager_build_line(c);
    }
}

/*
 * XXXXXX
 */
uint8_t
tty_manager_get_char(void)
{
    return bfifo_get(&g_bfifo);
}
