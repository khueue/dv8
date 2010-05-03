#ifndef LIFO_QUEUE_H
#define LIFO_QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
struct lifo_queue;
typedef struct lifo_queue lifo_queue_t;
struct lifo_queue
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
lifo_init_queue(lifo_queue_t *q);

void
lifo_enqueue(lifo_queue_t *q, void *data);

void *
lifo_dequeue(lifo_queue_t *q);

#endif
