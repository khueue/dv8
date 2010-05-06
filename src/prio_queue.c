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
prio_init_queue(prio_queue_t *q, int (*cmp_fun)(void *, void *))
{
    ZERO_STRUCT(q);
    q->cmp_fun = cmp_fun;
}

/*
 * Ordered insert in the prio_queue.
 */
void
prio_enqueue(prio_queue_t *q, void *data)
{
    list_node_t *new_node = list_node_alloc();

    /* kdebug_assert(q != NULL);
    kdebug_assert(data != NULL);
    kdebug_assert(new_node != NULL);*/

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

   /* kdebug_assert(q != NULL);
    kdebug_assert(q->length > 0); */

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
comparefun(void *a, void *b)
{
    int x = *(int *)a;
    int y = *(int *)b;
    return x - y;
}

#if 0
static void
testprint(prio_queue_t *q)
{
    list_node_t *tmp_node = q->head;
    while (tmp_node)
    {
        printf("%d, ", *(int *)tmp_node->data);
        tmp_node = tmp_node->next;
    }
    printf("\n");
}
#endif

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
#if 0
    int d1 = 10;
    int d2 = 20;
    int d3 = 30;
    int d4 = 5;
    int d5 = 15;
#endif

    int idle = 1;
    int fib = 20;

    prio_queue_t q;

    prio_init_queue(&q, &comparefun);
    print_queue(&q);

    prio_enqueue(&q, &idle);
    print_queue(&q);

    prio_enqueue(&q, &fib);
    print_queue(&q);

    prio_dequeue(&q);
    print_queue(&q);

    prio_dequeue(&q);
    print_queue(&q);

    #if 0
    prio_enqueue(&q, &d1);
    printf("enqueue: \"%d\"\n",d1);
    testprint(&q);
    prio_enqueue(&q, &d2);
    printf("enqueue: \"%d\"\n",d2);
    testprint(&q);
    prio_enqueue(&q, &d3);
    printf("enqueue: \"%d\"\n",d3);
    testprint(&q);
    prio_enqueue(&q, &d4);
    printf("enqueue: \"%d\"\n",d4);
    testprint(&q);
    prio_enqueue(&q, &d5);
    printf("enqueue: \"%d\"\n",d5);

    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));

    prio_enqueue(&q, &d1);
    printf("enqueue: \"%d\"\n",d1);
    testprint(&q);
    prio_enqueue(&q, &d2);
    printf("enqueue: \"%d\"\n",d2);
    testprint(&q);
    prio_enqueue(&q, &d3);
    printf("enqueue: \"%d\"\n",d3);
    testprint(&q);

    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);

    prio_enqueue(&q, &d4);
    printf("enqueue: \"%d\"\n",d4);
    prio_enqueue(&q, &d4);
    printf("enqueue: \"%d\"\n",d4);
    prio_enqueue(&q, &d4);
    printf("enqueue: \"%d\"\n",d4);
    testprint(&q);
    prio_enqueue(&q, &d5);
    printf("enqueue: \"%d\"\n",d5);
    testprint(&q);

    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    testprint(&q);
    printf("dequeue: \"%d\"\n", *(int *)prio_dequeue(&q));
    #endif
    return 0;
}

#endif
