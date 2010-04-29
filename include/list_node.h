#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Generic list node. Could be used as pretty much any type of list node. Just
 * add more pointers as needed.
 */
struct list_node;
typedef struct list_node list_node_t;
struct list_node
{
    void *data;
    list_node_t *next;
    list_node_t *prev;
    /* Add more useful pointers here ... */

    /* For the internal freelist. */
    list_node_t *next_free;
};

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
