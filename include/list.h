#ifndef LIST_H
#define LIST_H

#include "utils.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

typedef int (*compare_func)(const void *data1, const void *data2);
typedef int (*match_func)(const void *data, const void *id);

typedef struct _list list_t;

/* XXX should really be in the .c file */
struct _list
{
    list_node_t *head;
    list_node_t *foot;
    list_node_t *current;
    size_t length;
    compare_func compare;
    match_func is_match;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Initializes the list.
 */
void list_init(list_t *list, compare_func compare, match_func is_match);

/*
 * Inserts an item into the list. Returns true on success, false otherwise.
 * - head() inserts at the front of the list ("cons")
 * - foot() inserts at the back of the list
 * - ordered() inserts at the position determined by the list's compare_func
 */
int list_insert_head(list_t *list, void *data);
int list_insert_foot(list_t *list, void *data);
int list_insert_ordered(list_t *list, void *data);

/*
 * Removes and returns an item from the list.
 * - head() removes from the front of the list
 * - foot() removes from the back of the list
 * - remove() removes the first item (starting from the back) that matches id,
 *     using the list's match_func
 */
void *list_remove_head(list_t *list);
void *list_remove_foot(list_t *list);
void *list_remove(list_t *list, const void *id);

/*
 * Returns, but does not remove, an item from the list.
 * - head() returns the front of the list
 * - foot() returns the back of the list
 * - find() returns the first item (starting from the back) that matches id,
 *     using the list's match_func
 */
void *list_find_head(const list_t *list);
void *list_find_foot(const list_t *list);
void *list_find(const list_t *list, const void *id);

/*
 * Iterates over the list, starting at the front.
 * - reset() resets the iterator to the front of the list
 * - has_next() returns true if there are more items left to iterate over
 * - next() returns the next item in the iterator
 */
void  list_iter_reset(list_t *list);
int   list_iter_has_next(const list_t *list);
void *list_iter_next(list_t *list);

/*
 * Other useful functions.
 * - length() returns the length of the list
 * - is_empty() returns true if the list is empty, false otherwise
 */
size_t list_length(const list_t *list);
int    list_is_empty(const list_t *list);

/*
 * Functions useful for debugging.
 */
#ifdef UNITTEST
void dbg_print_list_node(const list_node_t *node);
void dbg_print_list(const list_t *list);
int  dbg_int_cmp(const void *a, const void *b);
int  dbg_int_eql(const void *a, const void *b);
#endif

#endif
