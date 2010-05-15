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

void   list_init(list_t *list, compare_func compare, match_func is_match);

int    list_insert_head(list_t *list, void *data);
int    list_insert_foot(list_t *list, void *data);
int    list_insert_ordered(list_t *list, void *data);

void * list_remove_head(list_t *list);
void * list_remove_foot(list_t *list);
void * list_remove(list_t *list, const void *id);

void * list_find_head(const list_t *list);
void * list_find_foot(const list_t *list);
void * list_find(const list_t *list, const void *id);

void   list_iter_reset(list_t *list);
int    list_iter_has_next(const list_t *list);
void * list_iter_next(list_t *list);

size_t list_length(const list_t *list);
int    list_is_empty(const list_t *list);

#ifdef UNITTEST
void   dbg_print_list_node(const list_node_t *node);
void   dbg_print_list(const list_t *list);
int    dbg_int_cmp(const void *a, const void *b);
int    dbg_int_eql(const void *a, const void *b);
#endif

#endif
