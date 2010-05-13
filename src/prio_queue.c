#include "utils.h"
#include "prio_queue.h"
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
    int (*cmp_fun)(const void *data1, const void *data2),
    int (*match_fun)(const void *data, const void *id))
{
    ZERO_STRUCT(q);
    q->cmp_fun = cmp_fun;
    q->match_fun = match_fun;
}

/*
 * Returns true if the queue is empty, false otherwise.
 */
int
prio_is_empty(const prio_queue_t *q)
{
    return q->length == 0;
}

/*
 * Returns, but does not remove, the first item (highest priority) in the
 * queue. Returns NULL if the queue is empty.
 */
void *
prio_peek(const prio_queue_t *q)
{
    kdebug_assert(q);

    return (prio_is_empty(q)) ? NULL : q->head->data;
}

/*
 * Ordered insert in the prio_queue.
 */
void
prio_enqueue(prio_queue_t *q, void *data)
{
    list_node_t *new_node = list_node_alloc();

    kdebug_assert(q);
    kdebug_assert(data);
    kdebug_assert(new_node);

    new_node->data = data;

    /* empty queue */
    if (!q->foot)
    {
        q->head = new_node;
        q->foot = new_node;
    }
    else
    {
        list_node_t *tmp_node = q->foot;
        /* find insert point */
        while (tmp_node && (q->cmp_fun(tmp_node->data, new_node->data) < 0))
        {
            tmp_node = tmp_node->prev;
        }

        /* first */
        if (!tmp_node)
        {
            q->head->prev = new_node;
            new_node->next = q->head;
            q->head = new_node;
        }
        /* last */
        else if (!tmp_node->next)
        {
            tmp_node->next = new_node;
            new_node->prev = tmp_node;
            q->foot = new_node;
        }
        else
        {
            new_node->next = tmp_node->next;
            new_node->prev = tmp_node;

            tmp_node->next = new_node;
            new_node->next->prev = new_node;
        }
    }
    q->length++;
 }

/*
 * Returns the first data of the queue.
 */
void *
prio_dequeue(prio_queue_t *q)
{
    list_node_t *node = q->head;
    void *data = node->data;

    kdebug_assert(q);
    kdebug_assert(q->length > 0);

    q->head = node->next;
    node = list_node_free(node);

    q->length--;

    if (!q->length)
    {
        q->foot = NULL;
        q->head = NULL;
    }
    if (q->head)
    {
        q->head->prev = NULL;
    }

    return data;
}

/*
 * Returns and removes the found data from the queue.
 */
void *
prio_remove(prio_queue_t *q, const void *id)
{
    list_node_t *node = q->foot;
    void *data = NULL;

    kdebug_assert(q != NULL);

    /* While when still in queue or requested node is found */
    while (node && !q->match_fun(node->data, id))
    {
        node = node->prev;
    }

    if (node)
    {
        if (node->prev)
        {
            node->prev->next = node->next;
        }
        else
        {
            q->head = node->next;
        }

        if (node->next)
        {
            node->next->prev = node->prev;
        }
        else
        {
            q->foot = node->prev;
        }

        data = node->data;
        node = list_node_free(node);

        q->length--;

        if (!q->length)
        {
            q->foot = NULL;
            q->head = NULL;
        }
    }

    return data;
}

/*
 * Returns, but does not remove, the first item from the end of the queue that
 * matches the given id.
 */
void *
prio_find(const prio_queue_t *q, const void *id)
{
    list_node_t *node = NULL;

    kdebug_assert(q);

    node = q->foot;
    while (node && !q->match_fun(node->data, id))
    {
        node = node->prev;
    }

    return (node) ? node->data : NULL;
}

void
prio_iterator_reset(prio_queue_t *q)
{
    q->current = q->head;
}

int
prio_iterator_has_next(prio_queue_t *q)
{
    return q->current != NULL;
}

void *
prio_iterator_next(prio_queue_t *q)
{
    void *data = NULL;
    
    kdebug_assert(q);
    kdebug_assert(q->current);
    
    data = q->current->data;
    q->current = q->current->next;
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
