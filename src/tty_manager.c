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

static char
g_line_buf[STR_BUF_SIZE];

static size_t
g_line_buf_pos;

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
tty_manager_subscribe_to_input(pcb_t *pcb)
{
    stack_push(&g_input_stack, pcb);
}

/*
 * XXXXX
 */
void
tty_manager_unsubscribe_from_input(pcb_t *pcb)
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

    /* Add to buffer only if there is room. */
    if (g_line_buf_pos < sizeof(g_line_buf))
    {
        g_line_buf[g_line_buf_pos++] = c;
    }

    /* If the last character pressed was a return, proceed to finish. */
    if (c == '\r')
    {
        /* Terminate both the newline and at end of string, to be safe. */
        g_line_buf[g_line_buf_pos-1] = '\0';
        g_line_buf[sizeof(g_line_buf)-1] = '\0';
        g_line_buf_pos = 0;

        /* Send input message to process if we have any input listeners. */
        if (g_input_stack.length > 0)
        {
            pcb_t *process = NULL;
            msg_t *msg = msg_alloc();
            int x;
            if (!msg)
            {
                /* Fail! XXXXXX */
            }
            msg->type = MSG_TYPE_CONSOLE_INPUT;
            x = atoi(g_line_buf);
            if (x != 0 || strcmp(g_line_buf, "0") == 0)
            {
                msg->data_type = MSG_DATA_TYPE_INTEGER;
                msg->data.integer = x;
            }
            else
            {
                msg->data_type = MSG_DATA_TYPE_STRING;
                strcpy(msg->data.string, g_line_buf);
            }
            process = stack_peek(&g_input_stack);
            fifo_enqueue(&process->inbox_q, msg);
            kunblock(process->pid);
        }
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
