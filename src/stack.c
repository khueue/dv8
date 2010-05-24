#include "utils.h"
#include "stack.h"
#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
stack_init(stack_t *stack, compare_func compare, match_func is_match)
{
    list_init(stack, compare, is_match);
}

void
stack_push(stack_t *stack, void *data)
{
    list_insert_head(stack, data);
}

void *
stack_pop(stack_t *stack)
{
    return list_remove_head(stack);
}

void *
stack_remove(stack_t *stack, void *id)
{
    return list_remove(stack, id);
}

void *
stack_peek(const stack_t *stack)
{
    return list_find_head(stack);
}

int
stack_is_empty(const stack_t *stack)
{
    return list_is_empty(stack);
}

size_t
stack_length(const stack_t *stack)
{
    return list_length(stack);
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DSTACK_MAIN src/stack.c \
    src/utils.c src/list_node.c src/list.c src/pcb.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef STACK_MAIN

#include <stdlib.h>
#include <stdio.h>

#include "pcb.h"

int
main(void)
{
    int d1 = 10;
    int d2 = 20;
    int d3 = 30;

    stack_t stack;

    stack_init(&stack, pcb_cmp_priority, pcb_has_pid);

    printf("push: \"%d\"\n",d1);
    stack_push(&stack, &d1);
    print_list(&stack);

    printf("push: \"%d\"\n",d2);
    stack_push(&stack, &d2);
    print_list(&stack);

    printf("push: \"%d\"\n",d3);
    stack_push(&stack, &d3);
    print_list(&stack);

    printf("pop: \"%d\"\n", *(int *)stack_pop(&stack));
    print_list(&stack);

    printf("pop: \"%d\"\n", *(int *)stack_pop(&stack));
    print_list(&stack);

    printf("push: \"%d\"\n",d3);
    stack_push(&stack, &d3);
    print_list(&stack);

    printf("pop: \"%d\"\n", *(int *)stack_pop(&stack));
    print_list(&stack);

    printf("pop: \"%d\"\n", *(int *)stack_pop(&stack));
    print_list(&stack);

    printf("push: \"%d\"\n",d1);
    stack_push(&stack, &d1);
    print_list(&stack);

    printf("push: \"%d\"\n",d2);
    stack_push(&stack, &d2);
    print_list(&stack);

    printf("push: \"%d\"\n",d3);
    stack_push(&stack, &d3);
    print_list(&stack);

    printf("peek: \"%d\"\n", *(int *)stack_peek(&stack));
    print_list(&stack);

    printf("remove 20:\n");
    stack_remove(&stack, &d2);
    print_list(&stack);

    printf("pop:\n");
    stack_pop(&stack);
    print_list(&stack);

    printf("pop:\n");
    stack_pop(&stack);
    print_list(&stack);

    return 0;
}

#endif
