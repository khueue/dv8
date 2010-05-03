#ifndef PRIO_QUEUE_H
#define PRIO_QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Priority queue struct where cmp_fun is the compare function.
 */
typedef struct prio_queue prio_queue_t;
struct prio_queue
{
    list_node_t *head;
    list_node_t *foot;
    int (*cmp_fun)(void *, void *);
    size_t length;
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
