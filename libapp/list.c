#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * libapp: an utility library to simplify
 * common tasks in app development.
 * (C) 2010 - Domenico Rotiroti
 * Licensed under the LGPL 3.0
 *
 * list.c: a very simple linked list that's actually usable
 * (hooray for iterators, boo for walkers)
 */

struct node_t {
	void   * data;
	struct node_t * next;
};

struct list_t {
	node * first, * last;
	int    size;
	list_deallocator deallocator;
};


node *   node_new( void * data)
{
	node * n = malloc(sizeof(node));
	n->data = data;
	n->next = NULL;
	
	return n;
}

void    node_free(list * l, node * n)
{
	if(l->deallocator) l->deallocator(n->data);
	free(n);
}

int pointer_comparator(void * a, void *b)
{
	if (a<b) return -1;
	return (a==b ? 0 : 1);
}

list *   list_new() { return list_new_full(NULL); }
list *   list_new_full(list_deallocator dealloc)
{
	list * l = (list*) malloc(sizeof(list));
	
	l->first = l->last = NULL;
	l->size = 0;
	l->deallocator = dealloc;
	
	return l;
}

void     list_free(list * l)
{
	list_iter i = list_get_iter(l), cur;

	while(i) {
		cur = i;
		i = list_iter_next(i);
		node_free(l,cur);
	}
	
	free(l);
}

void     list_append(list * l, void * item)
{
	node * n = node_new(item);
	if(l->last) l->last->next = n;
	l->last = n;
	if(!l->size) l->first = l->last;
	l->size++;
}

void       list_prepend(list * l, void * item)
{
	node * n = node_new(item);
	n->next = l->first;
	l->first = n;
	if(!l->size) l->last = l->first;
	l->size++;	
}

void       list_insert_at(list * l, void * item, int at)
{
	list_iter it;
	node * n;
	int i=0;
	ASSERT( (at >=0 && at < l->size) )

	if(at==0) return list_prepend(l, item);
	it = list_get_iter(l);
	for(; i<at-1; ++i) it = list_iter_next(it);
	n = node_new(item);
	n->next = it->next;
	it->next = n;
	if(n->next==NULL) l->last=n;
	l->size++;
}

void       list_insert_sorted(list *l, void * item)
{
	list_insert_sorted_comp(l, item, pointer_comparator);
}
void       list_insert_sorted_comp(list *l, void * item, list_comparator lc)
{
	list_iter it;
	node * n;
	
	if(l->size==0) return list_prepend(l, item);
	if(lc(item, l->first->data)<=0) return list_prepend(l, item);

	it = list_get_iter(l);
	while(it->next) {
		if(lc(item,it->next->data)<=0) break;
		it = list_iter_next(it);
	}
    n = node_new(item);
    n->next = it->next;
    it->next = n;
    if(n->next==NULL) l->last=n;
    l->size++;            
}

void       list_delete_first(list *l)
{
	node * n;
	ASSERT (l->size > 0)
	
	n = l->first;
	l->first = l->first->next;
	node_free(l,n);
	l->size--;
	if(l->size==0) l->last=NULL;
}

void  list_delete_node(list *l,  list_iter it)
{
	node * n = it->next;
	it->next = n->next;
	if(l->last==n) l->last=it;
	node_free(l,n);
	l->size--;
}

void       list_delete_at(list * l, int at)
{
    list_iter it;
    int i =0;
    ASSERT( (at >=0 && at < l->size) )

	if(at==0) return list_delete_first(l);
	for(it = list_get_iter(l); i<at-1; ++i) it = list_iter_next(it);
	list_delete_node(l, it);
}

bool       list_delete_item(list * l, void * item)
{
	return list_delete_item_comp(l, item, pointer_comparator);
}

bool       list_delete_item_comp(list *l, void * item, list_comparator lc)
{
	list_iter it;
	
	if(!l || l->size==0) return false;
	if(lc(item,l->first->data)==0) {
		list_delete_first(l);
		return true;
	}
	it = list_get_iter(l);
	while(it->next) {
		if(lc(item,it->next->data)==0) break;
		it = list_iter_next(it);
	}
	if(it->next) {
		list_delete_node(l, it);
		return true;
	}
	return false;
}

int        list_size(list * l)
{
	return l->size;
}

list_iter  list_get_iter(list * l)
{
	if(l) return l->first;
	return NULL;
}

list_iter  list_iter_next(list_iter iter)
{
	if(iter) return (list_iter) iter->next;
	return NULL;
}

void *     list_iter_getval(list_iter iter)
{
	if(iter) return iter->data;
	return NULL;
}
