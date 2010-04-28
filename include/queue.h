#ifndef QUEUE_H
#define QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
struct queue;
typedef struct queue queue_t;
struct queue
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
init_queue(queue_t *q);

void
enqueue(queue_t *q, void *data);

void *
dequeue(queue_t *q);

#endif
