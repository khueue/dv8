#ifndef LIFO_QUEUE_H
#define LIFO_QUEUE_H

#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXX
 */
typedef struct stack stack_t;
struct stack
{
    list_node_t *head;
    list_node_t *foot;
    size_t length;
    int (*is_match)(void *data, void *id);
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
stack_init(stack_t *stack, int (*is_match)(void *data, void *id));

void
stack_push(stack_t *stack, void *data);

void *
stack_pop(stack_t *stack);

void *
stack_remove(stack_t *stack, void *date);

void *
stack_peek(const stack_t *stack);

#endif
