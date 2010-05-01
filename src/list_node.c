#include "utils.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

#ifndef NUM_LIST_NODES
#define NUM_LIST_NODES 1024
#endif

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Generic list node. Could be used as pretty much any type of list node. Just
 * add more pointers as needed.
 */
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
 * Globals.
 * ---------------------------------------------------------------------------
 */

/*
 * All list nodes available in the system, organised on a freelist.
 */
static list_node_t
g_list_nodes[NUM_LIST_NODES];

/*
 * Freelist pointer. Points to the first free node.
 */
static list_node_t *
g_list_node_freelist;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the freelist like a normal linked list. This function must be
 * called exactly once before calling any other list node functions.
 */
void
init_list_node_freelist(void)
{
    size_t i = 0;

    g_list_node_freelist = &g_list_nodes[0];

    for (i = 0; i < COUNT_ARRAY(g_list_nodes)-1; ++i)
    {
        g_list_nodes[i].next_free = &g_list_nodes[i+1];
    }

    g_list_nodes[COUNT_ARRAY(g_list_nodes)-1].next_free = NULL;
}

/*
 * Returns a free list node, or NULL if none free.
 */
list_node_t *
alloc_list_node(void)
{
    list_node_t *node = g_list_node_freelist;
    if (!node)
    {
        return NULL;
    }
    else
    {
        g_list_node_freelist = g_list_node_freelist->next_free;
        return ZERO_STRUCT(node);
    }
}

/*
 * Releases a list node back to the system. Always returns NULL, to make it
 * easy and idiomatic to avoid dangling pointers:
 *
 *   node = alloc_list_node();
 *   ... use node ...
 *   node = free_list_node();
 */
list_node_t *
free_list_node(list_node_t *node)
{
    node->next_free = g_list_node_freelist;
    g_list_node_freelist = node;
    return NULL;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
 * Define this constant and compile this and any required modules, e.g.:
 *
 *   gcc <this_module>.c <other_modules>.c -D<THIS_MODULE>_MAIN -Iinclude
 *   ./a.out
 */
#ifdef LIST_NODE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    list_node_t *node = NULL;

    init_list_node_freelist();

    node = alloc_list_node();
    node->data = "hej";
    printf("node->data: \"%s\"\n", (char *)node->data);
    node = free_list_node(node);

    /* Allocate again, should be the same struct but zeroed. */
    node = alloc_list_node();
    printf("node->data: \"%s\"\n", (char *)node->data); /* Should segfault! */
    node = free_list_node(node);

    return 0;
}

#endif
