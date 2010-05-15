#include "utils.h"
#include "fifo_queue.h"
#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
fifo_init(fifo_queue_t *q, compare_func compare, match_func is_match)
{
    list_init(q, compare, is_match);
}

void
fifo_enqueue(fifo_queue_t *q, void *data)
{
    list_insert_foot(q, data);
}

void *
fifo_dequeue(fifo_queue_t *q)
{
    return list_remove_head(q);
}
