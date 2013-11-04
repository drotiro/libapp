#ifndef APP_LIST_H
#define APP_LIST_H

#include <libapp/base.h>

/*
 * libapp: an utility library to simplify
 * common tasks in app development.
 * (C) 2010-2013 - Domenico Rotiroti
 * Licensed under the LGPL 3.0
 *
 * list.h: a very simple linked list that's actually usable
 * (hooray for iterators, boo for walkers)
 */

typedef struct node_t node;
typedef void (*list_deallocator)(void * data);
typedef int (*list_comparator)(void * a, void *b);
typedef struct list_t list;

typedef node * list_iter;

list *     list_new();
list *     list_new_full(list_deallocator dealloc);
void       list_free(list * l);

void       list_append(list * l, void * item);
void       list_prepend(list * l, void * item);
void       list_insert_at(list * l, void * item, int at);
void       list_insert_sorted(list *l, void * item);
void       list_insert_sorted_comp(list *l, void * item, list_comparator lc);

void       list_delete_first(list *l);
void       list_delete_at(list * l, int at);
bool       list_delete_item(list * l, void * item);
bool       list_delete_item_comp(list *l, void * item, list_comparator lc);

int        list_size(list * l);

list_iter  list_get_iter(list * l);
list_iter  list_iter_next(list_iter iter);
void *     list_iter_getval(list_iter iter);



#endif //APP_LIST_H
