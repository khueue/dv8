#include "utils.h"
#include "malta.h"
#include "asm.h"
#include "tty_manager.h"
#include "stack.h"

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
  uint32_t pos_free;
};

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

#if 0
/*
 * XXXXX processes wanting input
 */
static stack_t
g_input_stack;

#endif

static char
str_buf[STR_BUF_SIZE];

static char *
str_buf_pos = str_buf;

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

    if (bfifo->pos_free < COUNT_ARRAY(bfifo->buf))
    {
        bfifo->buf[bfifo->pos_free++] = c;
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
    kdebug_assert(bfifo->pos_free > 0);

    c = bfifo->buf[0];
    --bfifo->pos_free;
    for (i = 0; i < bfifo->pos_free; ++i)
    {
        bfifo->buf[i] = bfifo->buf[i+1];
    }

    return c;
}

/*
 * XXXXXXXX
 */
int
tty_manager_has_characters(void)
{
    return g_bfifo.pos_free > 0;
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

    *str_buf_pos++ = c;
    if (c == '\r')
    {
        *str_buf_pos++ = '\n';
        *str_buf_pos++ = '\0';
        str_buf_pos = str_buf;
        /* copy str_buf into some top of stack process' message XXXXXXXXX */
        kdebug_println("");
        kdebug_print("str_buf: ");
        kdebug_println(str_buf);
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
