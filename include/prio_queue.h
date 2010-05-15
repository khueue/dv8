#ifndef PRIO_QUEUE_H
#define PRIO_QUEUE_H

#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

typedef list_t prio_queue_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
prio_init_queue(prio_queue_t *q, compare_func compare, match_func is_match);

int
prio_is_empty(const prio_queue_t *q);

size_t
prio_length(const prio_queue_t *q);

void
prio_enqueue(prio_queue_t *q, void *data);

void *
prio_dequeue(prio_queue_t *q);

void *
prio_remove(prio_queue_t *q, const void *id);

void *
prio_find(const prio_queue_t *q, const void *id);

void
prio_iter_reset(prio_queue_t *q);

int
prio_iter_has_next(const prio_queue_t *q);

void *
prio_iter_next(prio_queue_t *q);

#endif
