#include "utils.h"
#include "lifo_queue.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Init the queue.
 */
void
lifo_init_queue(lifo_queue_t *q)
{
    ZERO_STRUCT(q);
}

/*
 * Insert first in the lifo_queue.
 */
void
lifo_enqueue(lifo_queue_t *q, void *data)
{
    list_node_t *new_node = alloc_list_node();

    new_node->data = data;
    new_node->next = q->head;
    q->head = new_node;
}

/*
 * Returns the first data of the queue.
 */
void *
lifo_dequeue(lifo_queue_t *q)
{
    list_node_t *node = q->head;
    void *data = node->data;

    q->head = node->next;
    free_list_node(node);

    return data;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
 * Define this constant and compile this and required modules, e.g.:
 *   gcc <this_module>.c <other_modules>.c -D<THIS_MODULE>_MAIN -Iinclude
 *   ./a.out
 */
#ifdef LIFO_QUEUE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    double d1 = 10.0;
    double d2 = 20.0;
    double d3 = 30.0;

    lifo_queue_t q;

    init_list_node_freelist();

    lifo_init_queue(&q);
    lifo_enqueue(&q, &d1);
    lifo_enqueue(&q, &d2);
    lifo_enqueue(&q, &d3);

    printf("q->head->data: \"%f\"\n", *(double *)q.head->data);
    printf("dequeue: \"%f\"\n", *(double *)lifo_dequeue(&q));
    printf("dequeue: \"%f\"\n", *(double *)lifo_dequeue(&q));
    printf("dequeue: \"%f\"\n", *(double *)lifo_dequeue(&q));

    return 0;
}

#endif