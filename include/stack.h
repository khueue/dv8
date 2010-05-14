#ifndef STACK_H
#define STACK_H

#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXX
 */
typedef list_t stack_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
stack_init(
    stack_t *stack,
    int (*compare)(const void *data1, const void *data2),
    int (*is_match)(const void *data, const void *id));

void
stack_push(stack_t *stack, void *data);

void *
stack_pop(stack_t *stack);

void *
stack_remove(stack_t *stack, void *id);

void *
stack_peek(const stack_t *stack);

size_t stack_length(const stack_t *stack);
int    stack_is_empty(const stack_t *stack);

#endif
