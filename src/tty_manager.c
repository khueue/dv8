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
    stack_remove(&g_input_stack, pcb);
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
 * XXXXX
 */
static msg_t *
tty_manager_create_message(const char str[])
{
    msg_t *msg = NULL;

    msg = msg_alloc();
    if (!msg)
    {
        /* Fail! XXXXXX */
    }

    msg->type = MSG_TYPE_CONSOLE_INPUT;
    msg->data.integer = atoi(str);
    if (msg->data.integer != 0 || strcmp(str, "0") == 0)
    {
        msg->data_type = MSG_DATA_TYPE_INTEGER;
    }
    else
    {
        msg->data_type = MSG_DATA_TYPE_STRING;
        strcpy(msg->data.string, str);
    }

    return msg;
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
        msg_t *msg = NULL;
        pcb_t *process = NULL;

        msg = tty_manager_create_message(str);
        process = stack_peek(&g_input_stack);
        fifo_enqueue(&process->inbox_q, msg);
        kunblock(process->pid);
    }
}

/*
 * Successively builds a complete line of characters from input.
 */
static void
tty_manager_build_line(uint8_t c)
{
    static char buf[STR_BUF_SIZE] = { '\0' };
    static size_t pos = 0;

    /* Add to buffer only if there is room. */
    if (pos < sizeof(buf))
    {
        buf[pos++] = c;
    }

    /* If the last character received was a return, proceed to finish. */
    if (c == '\r')
    {
        /* Terminate both the newline and the end of string, to be safe. */
        buf[pos-1] = '\0';
        buf[sizeof(buf)-1] = '\0';
        pos = 0;

        tty_manager_dispatch_message(buf);
    }
}

/*
 * XXXXXX
 */
void
tty_manager_put_char(uint8_t c)
{
    bfifo_put(&g_bfifo, c);
    if (c == '\r')
    {
        bfifo_put(&g_bfifo, '\n');
    }

    tty_manager_build_line(c);
}

/*
 * XXXXXX
 */
uint8_t
tty_manager_get_char(void)
{
    return bfifo_get(&g_bfifo);
}
