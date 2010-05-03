#ifndef LIFO_QUEUE_H
#define LIFO_QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Priority queue struct where cmp_fun is the compare function.
 */
struct prio_queue;
typedef struct prio_queue prio_queue_t;
struct prio_queue
{
    list_node_t *head;
    list_node_t *foot;
    int (*cmp_fun)(void *, void *);
    int length;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
prio_init_queue(prio_queue_t *q, int (*cmp_fun)(void *, void *));

void
prio_enqueue(prio_queue_t *q, void *data);

void *
prio_dequeue(prio_queue_t *q);

#endif
