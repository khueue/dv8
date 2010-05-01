#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

typedef struct list_node list_node_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
init_list_node_freelist(void);

list_node_t *
alloc_list_node(void);

list_node_t *
free_list_node(list_node_t *node);

#endif
