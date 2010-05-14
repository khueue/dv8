#include "utils.h"
#include "list.h"
#include "list_node.h"

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

struct _list
{
    list_node_t *head;
    list_node_t *foot;
    list_node_t *current;
    size_t length;
    int (*compare)(const void *data1, const void *data2);
    int (*is_match)(const void *data, const void *id);
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
list_init(
    list_t *list,
    int (*compare)(const void *data1, const void *data2),
    int (*is_match)(const void *data, const void *id))
{
    ZERO_STRUCT(list);

    list->compare  = compare;
    list->is_match = is_match;
}

int
list_insert_head(list_t *list, void *data)
{
    list_node_t *node = NULL;

    kdebug_assert(list);
    kdebug_assert(data);

    node = list_node_alloc();
    if (!node)
    {
        /* XXX maybe print some stuff to the screen? */
        return 0;
    }

    node->data = data;

    if (list_is_empty(list))
    {
        list->head = list->foot = node;
    }
    else
    {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
        ++list->length;
    }

    return 1;
}

int
list_insert_foot(list_t *list, void *data)
{
    list_node_t *node = NULL;

    kdebug_assert(list);
    kdebug_assert(data);

    node = list_node_alloc();
    if (!node)
    {
        /* XXX maybe print some stuff to the screen? */
        return 0;
    }

    node->data = data;

    if (list_is_empty(list))
    {
        list->head = list->foot = node;
    }
    else
    {
        list->foot->next = node;
        node->prev = list->foot;
        list->foot = node;
        ++list->length;
    }

    return 1;
}

int
list_insert_ordered(list_t *list, void *data)
{
    list_node_t *node = NULL;

    kdebug_assert(list);
    kdebug_assert(data);

    node = list_node_alloc();
    if (!node)
    {
        /* XXXXXXXXXXX */
        return 0;
    }

    node->data = data;

    /* empty list */
    if (list_is_empty(list))
    {
        list->head = node;
        list->foot = node;
    }
    else
    {
        list_node_t *tmp_node = list->foot;
        /* find insert point */
        while (tmp_node && (list->compare(tmp_node->data, node->data) < 0))
        {
            tmp_node = tmp_node->prev;
        }

        /* first */
        if (!tmp_node)
        {
            list->head->prev = node;
            node->next = list->head;
            list->head = node;
        }
        /* last */
        else if (!tmp_node->next)
        {
            tmp_node->next = node;
            node->prev = tmp_node;
            list->foot = node;
        }
        else
        {
            node->next = tmp_node->next;
            node->prev = tmp_node;

            tmp_node->next = node;
            node->next->prev = node;
        }
    }
    ++list->length;

    return 1;
}

void *
list_remove_head(list_t *list)
{
    list_node_t *node = NULL;
    void *data = NULL;

    kdebug_assert(list);

    if (list_is_empty(list))
    {
        /* XXXXXX */
        return NULL;
    }

    node = list->head;
    data = node->data;

    if (list_length(list) == 1)
    {
        list->head = list->foot = NULL;
    }
    else
    {
        node->next->prev = NULL;
        list->head = node->next;
    }

    node = list_node_free(node);
    --list->length;

    return data;
}

void *
list_remove_foot(list_t *list)
{
    list_node_t *node = NULL;
    void *data = NULL;

    kdebug_assert(list);

    if (list_is_empty(list))
    {
        /* XXXXXX */
        return NULL;
    }

    node = list->foot;
    data = node->data;

    if (list_length(list) == 1)
    {
        list->head = list->foot = NULL;
    }
    else
    {
        node->prev->next = NULL;
        list->foot = node->prev;
    }

    node = list_node_free(node);
    --list->length;

    return data;
}

void *
list_remove(list_t *list, const void *id)
{
    list_node_t *node = NULL;
    void *data = NULL;

    kdebug_assert(list);
    kdebug_assert(id);

    node = list->foot;

    while (node && !list->is_match(node->data, id))
    {
        node = node->prev;
    }

    if (node)
    {
        if (node->prev)
        {
            node->prev->next = node->next;
        }
        else
        {
            list->head = node->next;
        }

        if (node->next)
        {
            node->next->prev = node->prev;
        }
        else
        {
            list->foot = node->prev;
        }

        data = node->data;
        node = list_node_free(node);

        --list->length;

        /* XZZZZZZZZZZZZZZZZZZZZ XXXXXXXXXX */
        if (list_is_empty(list))
        {
            list->foot = NULL;
            list->head = NULL;
        }
    }

    return data;
}

void *
list_find_head(const list_t *list)
{
    kdebug_assert(list);

    return (list->length == 0) ? NULL : list->head->data;
}

void *
list_find_foot(const list_t *list)
{
    kdebug_assert(list);

    return (list->length == 0) ? NULL : list->foot->data;
}

void *
list_find(const list_t *list, const void *id)
{
    list_node_t *node = NULL;

    kdebug_assert(list);
    kdebug_assert(id);

    node = list->foot;
    while (node && !list->is_match(node->data, id))
    {
        node = node->prev;
    }

    return (node) ? node->data : NULL;
}

void
list_iter_reset(list_t *list)
{
    kdebug_assert(list);

    list->current = list->head;
}

int
list_iter_has_next(list_t *list)
{
    kdebug_assert(list);

    return list->current != NULL;
}

void *
list_iter_next(list_t *list)
{
    void *data = NULL;

    kdebug_assert(list);
    kdebug_assert(list->current);

    data = list->current->data;
    list->current = list->current->next;
    return data;
}

size_t
list_length(const list_t *list)
{
    kdebug_assert(list);

    return list->length;
}

int
list_is_empty(const list_t *list)
{
    kdebug_assert(list);

    return list->length == 0;
}

/*
 * ---------------------------------------------------------------------------
 * Main for module testing.
 * ---------------------------------------------------------------------------
 */

/*
    gcc -DUNITTEST -DLIST_MAIN  \
    src/list.c src/list_node.c src/utils.c  \
    -Iinclude -W -Wall -Werror -Wshadow -Wpointer-arith \
    -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ansi -pedantic
 */
#ifdef LIST_MAIN

#include <stdlib.h>
#include <stdio.h>

static int
comparefun(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return x - y;
}

static int
findfun(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return x == y;
}

static void
print_list_node(list_node_t *node)
{
    printf("\n");

    if (node)
    {
        printf("\tList Node %d:\n", *(int *)node->data);
    }
    else
    {
        printf("\tList Node:\n");
    }

    if (!node)
    {
        printf("\t\tNode is NULL!!!\n");
    }
    else
    {
        if (node->prev)
        {
            printf("\t\tprev: %d\n", *(int *)node->prev->data);
        }
        else
        {
            printf("\t\tprev: NULL\n");
        }

        if (node->next)
        {
            printf("\t\tnext: %d\n", *(int *)node->next->data);
        }
        else
        {
            printf("\t\tnext: NULL\n");
        }
    }
}

static void
print_list(list_t *list)
{
    printf("List:\n");

    if (!list)
    {
        printf("\tQueue is NULL!!!\n");
    }
    else
    {
        printf("\tlength: %d\n", list->length);

        if (list->head)
        {
            printf("\thead: %d\n", *(int *)list->head->data);
        }
        else
        {
            printf("\thead: NULL\n");
        }

        if (list->foot)
        {
            printf("\tfoot: %d\n", *(int *)list->foot->data);
        }
        else
        {
            printf("\tfoot: NULL\n");
        }

        if (list->head)
        {
            list_node_t *tmp = list->head;
            while (tmp)
            {
                print_list_node(tmp);
                tmp = tmp->next;
            }
        }
    }

    printf("\n");
    printf("========================================================\n\n");
}

int
main(void)
{
    int one   = 1;
    int two   = 2;
    int three = 3;

    list_t q;

    list_init(&q, comparefun, findfun);

    printf("*** Empty list ***\n");
    print_list(&q);

    printf("*** Enqueue 2 ***\n");
    list_insert_ordered(&q, &two);
    print_list(&q);

    printf("*** Enqueue 1 ***\n");
    list_insert_ordered(&q, &one);
    print_list(&q);

    printf("*** Dequeue (2) ***\n");
    list_remove_head(&q);
    print_list(&q);

    printf("*** Dequeue (1) ***\n");
    list_remove_head(&q);
    print_list(&q);

    printf("*** Insert 3 remove 3 ***\n");

    list_insert_ordered(&q, &three);
    print_list(&q);

    list_remove(&q, &three);
    print_list(&q);

    printf("*** Insert 3 try remove 2 ***\n");
    list_insert_ordered(&q, &three);
    print_list(&q);

    list_remove(&q, &two);
    print_list(&q);

    printf("*** Insert 2,3 remove 2 ***\n");
    list_insert_ordered(&q, &two);
    print_list(&q);

    list_remove(&q, &two);
    print_list(&q);

    printf("*** Insert 2,3 remove 3 ***\n");
    list_insert_ordered(&q, &two);
    print_list(&q);

    list_remove(&q, &three);
    print_list(&q);

    printf("*** Insert 1,2,3 remove 2 ***\n");
    list_insert_ordered(&q, &one);
    list_insert_ordered(&q, &three);
    print_list(&q);

    list_remove(&q, &two);
    print_list(&q);

    printf("*** Insert 1,3 remove 1,3 ***\n");
    list_remove(&q, &one);
    list_remove(&q, &three);
    print_list(&q);

    return 0;
}

#endif
