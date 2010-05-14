#include "utils.h"
#include "prio_queue.h"
#include "list.h"
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
prio_init_queue(
    prio_queue_t *q,
    int (*compare)(const void *data1, const void *data2),
    int (*is_match)(const void *data, const void *id))
{
    list_init(q, compare, is_match);
}

/*
 * Returns true if the queue is empty, false otherwise.
 */
int
prio_is_empty(const prio_queue_t *q)
{
    return list_is_empty(q);
}

/*
 * Returns the length of the queue.
 */
size_t
prio_length(const prio_queue_t *q)
{
    return list_length(q);
}

/*
 * Ordered insert in the prio_queue.
 */
void
prio_enqueue(prio_queue_t *q, void *data)
{
    list_insert_ordered(q, data);
}

/*
 * Returns the first data of the queue.
 */
void *
prio_dequeue(prio_queue_t *q)
{
    return list_remove_head(q);
}

/*
 * Returns and removes the found data from the queue.
 */
void *
prio_remove(prio_queue_t *q, const void *id)
{
    return list_remove(q, id);
}

/*
 * Returns, but does not remove, the first item from the end of the queue that
 * matches the given id.
 */
void *
prio_find(const prio_queue_t *q, const void *id)
{
    return list_find(q, id);
}

void
prio_iterator_reset(prio_queue_t *q)
{
    list_iter_reset(q);
}

int
prio_iterator_has_next(prio_queue_t *q)
{
    return list_iter_has_next(q);
}

void *
prio_iterator_next(prio_queue_t *q)
{
    return list_iter_next(q);
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
#ifdef PRIO_QUEUE_MAIN

#include <stdlib.h>
#include <stdio.h>

static int
comparefun(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return x - y;
}

static int
findfun(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return x == y;
}

static void
print_list_node(list_node_t *node)
{
    printf("\n");

    if (node)
    {
        printf("\tList Node %d:\n", *(int *)node->data);
    }
    else
    {
        printf("\tList Node:\n");
    }

    if (!node)
    {
        printf("\t\tNode is NULL!!!\n");
    }
    else
    {
        if (node->prev)
        {
            printf("\t\tprev: %d\n", *(int *)node->prev->data);
        }
        else
        {
            printf("\t\tprev: NULL\n");
        }

        if (node->next)
        {
            printf("\t\tnext: %d\n", *(int *)node->next->data);
        }
        else
        {
            printf("\t\tnext: NULL\n");
        }
    }
}

static void
print_queue(prio_queue_t *q)
{
    printf("Prio Queue:\n");

    if (!q)
    {
        printf("\tQueue is NULL!!!\n");
    }
    else
    {
        printf("\tlength: %d\n", q->length);

        if (q->head)
        {
            printf("\thead: %d\n", *(int *)q->head->data);
        }
        else
        {
            printf("\thead: NULL\n");
        }

        if (q->foot)
        {
            printf("\tfoot: %d\n", *(int *)q->foot->data);
        }
        else
        {
            printf("\tfoot: NULL\n");
        }

        if (q->head)
        {
            list_node_t *tmp = q->head;
            while (tmp)
            {
                print_list_node(tmp);
                tmp = tmp->next;
            }
        }
    }

    printf("\n");
    printf("========================================================\n\n");
}

int
main(void)
{

    int one   = 1;
    int two   = 2;
    int three = 3;

    prio_queue_t q;

    prio_init_queue(&q, comparefun, findfun);

    printf("*** Empty queue ***\n");
    print_queue(&q);

    printf("*** Enqueue 2 ***\n");
    prio_enqueue(&q, &two);
    print_queue(&q);

    printf("*** Enqueue 1 ***\n");
    prio_enqueue(&q, &one);
    print_queue(&q);

    printf("*** Dequeue (2) ***\n");
    prio_dequeue(&q);
    print_queue(&q);

    printf("*** Dequeue (1) ***\n");
    prio_dequeue(&q);
    print_queue(&q);

    printf("*** Insert 3 remove 3 ***\n");

    prio_enqueue(&q, &three);
    print_queue(&q);

    prio_remove(&q, &three);
    print_queue(&q);

    printf("*** Insert 3 try remove 2 ***\n");
    prio_enqueue(&q, &three);
    print_queue(&q);

    prio_remove(&q, &two);
    print_queue(&q);

    printf("*** Insert 2,3 remove 2 ***\n");
    prio_enqueue(&q, &two);
    print_queue(&q);

    prio_remove(&q, &two);
    print_queue(&q);

    printf("*** Insert 2,3 remove 3 ***\n");
    prio_enqueue(&q, &two);
    print_queue(&q);

    prio_remove(&q, &three);
    print_queue(&q);

    printf("*** Insert 1,2,3 remove 2 ***\n");
    prio_enqueue(&q, &one);
    prio_enqueue(&q, &three);
    print_queue(&q);

    prio_remove(&q, &two);
    print_queue(&q);

    printf("*** Insert 1,3 remove 1,3 ***\n");
    prio_remove(&q, &one);
    prio_remove(&q, &three);
    print_queue(&q);

    return 0;
}

#endif
