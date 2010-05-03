#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
struct fifo_queue;
typedef struct fifo_queue fifo_queue_t;
struct fifo_queue
{
    list_node_t *head;
    list_node_t *foot;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
fifo_init_queue(fifo_queue_t *q);

void
fifo_enqueue(fifo_queue_t *q, void *data);

void *
fifo_dequeue(fifo_queue_t *q);

#endif
