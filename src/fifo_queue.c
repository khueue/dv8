#include "utils.h"
#include "fifo_queue.h"
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
fifo_init_queue(fifo_queue_t *q)
{
    ZERO_STRUCT(q);
}

/*
 * Insert last in the fifo_queue.
 */
void
fifo_enqueue(fifo_queue_t *q, void *data)
{
    list_node_t *new_node = alloc_list_node();

    new_node->data = data;
    
    if (q->foot)
    {
    	q->foot->next = new_node;    	
    } 
    else 
    {
    	q->foot = new_node;
    	q->head = new_node;
    }
    new_node->prev = q->foot;    
    q->foot = new_node;
}

/*
 * Returns the first data of the queue.
 */
void *
fifo_dequeue(fifo_queue_t *q)
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
#ifdef FIFO_QUEUE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    double d1 = 10.0;
    double d2 = 20.0;
    double d3 = 30.0;

    fifo_queue_t q;

    init_list_node_freelist();

    fifo_init_queue(&q);
    fifo_enqueue(&q, &d1);
    fifo_enqueue(&q, &d2);
    fifo_enqueue(&q, &d3);

    printf("q->head->data: \"%f\"\n", *(double *)q.head->data);
    printf("dequeue: \"%f\"\n", *(double *)fifo_dequeue(&q));
    printf("dequeue: \"%f\"\n", *(double *)fifo_dequeue(&q));
    printf("dequeue: \"%f\"\n", *(double *)fifo_dequeue(&q));

    return 0;
}

#endif
