#include "utils.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Globals.
 * ---------------------------------------------------------------------------
 */

#ifndef NUM_LIST_NODES
#define NUM_LIST_NODES 1024
#endif

/*
 * All list nodes available in the system, organised as a freelist.
 */
static list_node_t
g_list_nodes[NUM_LIST_NODES];

#undef NUM_LIST_NODES

/*
 * Freelist pointer. Points to the first free list node.
 */
static list_node_t *
g_list_node_freelist;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the freelist. This function must be called before any freelist
 * usage.
 */
void
init_list_node_freelist(void)
{
    size_t i;

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
    g_list_node_freelist = g_list_node_freelist->next_free;
    return ZERO_STRUCT(node);
}

/*
 * Releases a list node back to the system.
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
 * Define this constant and compile only this module, e.g.:
 *   gcc <this_module>.c -D<THIS_MODULE>_MAIN -Iinclude
 *   ./a.out
 */
#ifdef LIST_NODE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    list_node_t *node;

    init_list_node_freelist();

    node = alloc_list_node();
    node->data = "hej";
    printf("node->data: \"%s\"\n", node->data);
    node = free_list_node(node);

    /* Allocate again, should be the same struct but zeroed. */
    node = alloc_list_node();
    printf("node->data: \"%s\"\n", node->data); /* Should segfault! */
    node = free_list_node(node);

    return 0;
}

#endif
