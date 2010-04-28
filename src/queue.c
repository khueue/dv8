#include "utils.h"
#include "queue.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXX
 */
void
init_queue(queue_t *q)
{
    ZERO_STRUCT(q);
}

/*
 * XXX
 */
void
enqueue(queue_t *q, void *data)
{
    list_node_t *new_node = alloc_list_node();

    new_node->data = data;
    q->foot->next = new_node; /* Special case if none present? */
    new_node->prev = q->foot;
    q->foot = new_node;
}

/*
 * XXX
 */
void *
dequeue(queue_t *q)
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
#ifdef QUEUE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    double d1 = 10.0;
    double d2 = 20.0;
    double d3 = 30.0;

    queue_t q;

    init_list_node_freelist();

    init_queue(&q);
    enqueue(&q, &d1); /* Currently segfaults. */
    enqueue(&q, &d2);
    enqueue(&q, &d3);

    printf("q->head->data: \"%f\"\n", *(double *)q.head->data);

    return 0;
}

#endif
