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
 * find_fun is the find function
 */
typedef struct prio_queue prio_queue_t;
struct prio_queue
{
    list_node_t *head;
    list_node_t *foot;
    list_node_t *current;
    int (*cmp_fun)(void *, void *);
    int (*match_fun)(void *, void *);
    size_t length;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
prio_init_queue(
    prio_queue_t *q,
    int (*cmp_fun)(void *, void *),
    int (*match_fun)(void *, void *));

void
prio_enqueue(prio_queue_t *q, void *data);

void *
prio_dequeue(prio_queue_t *q);

void *
prio_remove(prio_queue_t *q, void *find);

void *
prio_find(prio_queue_t *q, void *id);

void
prio_iterator_reset(prio_queue_t *q);

int
prio_iterator_has_next(prio_queue_t *q);

void *
prio_iterator_next(prio_queue_t *q);

#endif
