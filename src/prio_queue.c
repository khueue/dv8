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
    list_node_t *new_node = alloc_list_node();
    
    /* kdebug_assert(q != NULL);
    kdebug_assert(data != NULL); */
    
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
    free_list_node(node);    
    
    q->length--;

    if (!q->length)
    {
        q->foot = NULL;
        q->head = NULL;
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

int
comparefun(void *a, void *b)
{
    int x = *(int *)a;
    int y = *(int *)b;
    return x - y;
}

void
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

int
main(void)
{
    
    
    int d1 = 10;
    int d2 = 20;
    int d3 = 30;
    int d4 = 5;
    int d5 = 15;

    prio_queue_t q;

    init_list_node_freelist();

    prio_init_queue(&q, &comparefun);
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
    return 0;
}

#endif
