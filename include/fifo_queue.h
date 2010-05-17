#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "utils.h"
#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

typedef list_t fifo_queue_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
fifo_init(fifo_queue_t *q, compare_func compare, match_func is_match);

void
fifo_enqueue(fifo_queue_t *q, void *data);

void *
fifo_dequeue(fifo_queue_t *q);

void *
fifo_find(fifo_queue_t *q, const void *id);

#endif
