#include "utils.h"
#include "stack.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXXXXInit the queue.
 */
void
stack_init(stack_t *stack)
{
    ZERO_STRUCT(stack);
}

/*
 * XXXXXXXXXInsert first in the lifo_queue.
 */
void
stack_push(stack_t *stack, void *data)
{
    list_node_t *new_node = list_node_alloc();

    if (!new_node)
    {
        /* XXXXXXX no more list nodes! */
    }

    new_node->data = data;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->length++;
}

/*
 * XXXXXXReturns the first data of the queue.
 */
void *
stack_pop(stack_t *stack)
{
    list_node_t *node = NULL;
    void *data = NULL;

    kdebug_assert(stack);

    if (stack->length == 0)
    {
        /* XXXXXXX ojojojoj */
    }

    node = stack->head;
    data = node->data;

    stack->head = node->next;
    node = list_node_free(node);
    stack->length--;
    return data;
}



/*
 * XXXXXXReturns the first data of the queue.
 */
void *
stack_peek(stack_t *stack)
{
    kdebug_assert(stack);

    if (stack->length == 0)
    {
        /* XXXXXXX ojojojoj */
    }

    return stack->head->data;
}


/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DSTACK_MAIN src/stack.c \
    src/utils.c src/list_node.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef STACK_MAIN

#include <stdlib.h>
#include <stdio.h>

static void
stack_testprint(stack_t *stack)
{
    list_node_t *tmp_node = stack->head;
    while (tmp_node)
    {
        printf("%f, ", *(double *)tmp_node->data);
        tmp_node = tmp_node->next;
    }
    printf("\n");
}

int
main(void)
{
    double d1 = 10.0;
    double d2 = 20.0;
    double d3 = 30.0;

    stack_t stack;

    stack_init(&stack);
    stack_push(&stack, &d1);
    printf("push: \"%f\"\n",d1);
    stack_testprint(&stack);

    stack_push(&stack, &d2);
    printf("push: \"%f\"\n",d2);
    stack_testprint(&stack);

    stack_push(&stack, &d3);
    printf("push: \"%f\"\n",d3);
    stack_testprint(&stack);

    printf("pop: \"%f\"\n", *(double *)stack_pop(&stack));
    stack_testprint(&stack);
    printf("pop: \"%f\"\n", *(double *)stack_pop(&stack));
    stack_testprint(&stack);

    stack_push(&stack, &d3);
    printf("push: \"%f\"\n",d3);
    stack_testprint(&stack);

    printf("pop: \"%f\"\n", *(double *)stack_pop(&stack));
    stack_testprint(&stack);

    printf("pop: \"%f\"\n", *(double *)stack_pop(&stack));
    stack_testprint(&stack);

    stack_push(&stack, &d1);
    printf("push: \"%f\"\n",d1);
    stack_testprint(&stack);

    stack_push(&stack, &d2);
    printf("push: \"%f\"\n",d2);
    stack_testprint(&stack);

    stack_push(&stack, &d3);
    printf("push: \"%f\"\n",d3);
    stack_testprint(&stack);

    return 0;
}

#endif
