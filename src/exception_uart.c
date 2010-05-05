#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "utils.h"

#include "exception_uart.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXXXXXX
 */
#define FIFO_SIZE 8

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
  uint8_t  buf[FIFO_SIZE];
  uint32_t length;
};

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

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
bfifo_put(bounded_fifo_t* bfifo, uint8_t c)
{
    /* Make sure the 'bfifo' pointer is not 0. */
    kdebug_assert(bfifo != 0);

    if (bfifo->length < COUNT_ARRAY(bfifo->buf))
    {
        bfifo->buf[bfifo->length++] = c;
    }
}

/*
 * bfifo_get: Returns a character removed from the front of the queue.
 */
static uint8_t
bfifo_get(bounded_fifo_t* bfifo)
{
    size_t i = 0;
    uint8_t c = '\0';

    /* Make sure the 'bfifo' pointer is not 0, and that queue is not empty. */
    kdebug_assert(bfifo != 0);
    kdebug_assert(bfifo->length > 0);

    --bfifo->length;
    c = bfifo->buf[0];
    for (i = 0; i < bfifo->length; ++i)
    {
        bfifo->buf[i] = bfifo->buf[i+1];
    }

    return c;
}

/*
 * XXXXXXXX
 */
void
execute_uart(cause_reg_t cause)
{
    if (cause.field.ip & BIT2)
    {
        /* UART interrupt */
        if (tty->lsr.field.dr)
        {
            /* Data ready: add character to buffer */
            uint8_t ch = tty->rbr;
            bfifo_put(&g_bfifo, ch);
            if (ch == '\r')
            {
                bfifo_put(&g_bfifo, '\n');
            }
        }

        if ((g_bfifo.length > 0) && tty->lsr.field.thre)
        {
            /* Transmitter idle: transmit buffered character */
            tty->thr = bfifo_get(&g_bfifo);

            /* Determine if we should be notified when transmitter becomes idle */
            tty->ier.field.etbei = (g_bfifo.length > 0);
        }

        /* Acknowledge UART interrupt. */
        kset_cause(~0x1000, 0);
    }
}
