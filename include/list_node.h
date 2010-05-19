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

/*
 * Returns the number of free list nodes in the system.
 */
size_t
list_node_num_free(void);

list_node_t *
list_node_alloc(void);

list_node_t *
list_node_free(list_node_t *node);

#endif
