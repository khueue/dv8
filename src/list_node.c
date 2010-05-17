#include "utils.h"
#include "list_node.h"

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
g_freelist;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the freelist like a normal linked list.
 */
static void
list_node_init_freelist(void)
{
    size_t i = 0;

    g_freelist = &g_list_nodes[0];

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
list_node_alloc(void)
{
    static int is_initialized = 0;
    list_node_t *node = NULL;

    /* Make sure the free list is initialized automatically! */
    if (!is_initialized)
    {
        is_initialized = 1;
        list_node_init_freelist();
    }

    node = g_freelist;
    if (!node)
    {
        return NULL;
    }
    else
    {
        g_freelist = g_freelist->next_free;
        return ZERO_STRUCT(node);
    }
}

/*
 * Releases a list node back to the system. Always returns NULL, to make it
 * easy to avoid dangling pointers by simply using the return value in an
 * assignment.
 */
list_node_t *
list_node_free(list_node_t *node)
{
    kdebug_assert(node);
    
    node->next_free = g_freelist;
    g_freelist = node;
    return NULL;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DLIST_NODE_MAIN src/list_node.c \
    src/utils.c src/pcb.c \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef LIST_NODE_MAIN

#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    list_node_t *node = NULL;

    node = list_node_alloc();
    node->data = "Wohoo!";
    printf("node->data: \"%s\"\n", (char *)node->data);
    node = list_node_free(node);

    /* Allocate again, should be the same struct but zeroed. */
    node = list_node_alloc();
    printf("node->data: \"%s\"\n", (char *)node->data); /* Should segfault! */
    node = list_node_free(node);

    return 0;
}

#endif
