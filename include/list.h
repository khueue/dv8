#ifndef LIST_H
#define LIST_H

#include "utils.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

typedef struct _list list_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void list_init(
    list_t *list,
    int (*compare)(const void *data1, const void *data2),
    int (*is_match)(const void *data, const void *id));

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
int    list_iter_has_next(list_t *list);
void * list_iter_next(list_t *list);

size_t list_length(const list_t *list);
int    list_is_empty(const list_t *list);

#endif
