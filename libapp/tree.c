#include <libapp/tree.h>

#include <stdlib.h>

/*
 * libapp: an utility library to simplify
 * common tasks in app development.
 * (C) 2010-2013 - Domenico Rotiroti
 * Licensed under the LGPL 3.0
 *
 * tree.h: basic n-ary tree
 */

//typedef struct tree_node_t tree;
//typedef void (*tree_deallocator)(void * data);

//num tree_delete_mode { APPEND_TO_PARENT, DEL_SUBTREE };

struct tree_node_t {
	tree *    parent;
	void *    item;
	list * children;
};

tree *    tree_new(void * val)
{
	tree * t = malloc(sizeof(struct tree_node_t));
	t->parent = NULL;
	t->item   = val;
	t->children = list_new();
	
	return t;
}

tree *    tree_get_parent(tree * t)
{
	if(t) return t->parent;
	return NULL;
}
list_iter tree_get_children(tree * t)
{
	if(t) return list_get_iter(t->children);
	return NULL;
}

void *    tree_getval(tree * t)
{
	if(t) return t->item;
	return NULL;
}

tree*      tree_insert(tree * parent, void * item)
{
	tree * t;
	if(!parent) return NULL;
	t = tree_new(item);
	t->parent = parent;
	list_append(parent->children, t);
	
	return t;
}

void      tree_delete(tree *t, tree_delete_mode mode)
{
	tree_delete_dealloc(t, mode, NULL);
}

void      tree_delete_dealloc(tree *t, tree_delete_mode mode, tree_deallocator free_node)
{
	tree * c;
	list_iter it = list_get_iter(t->children);
	
	for(; it; it = list_iter_next(it)) {
		if(mode==APPEND_TO_PARENT) {
			c = list_iter_getval(it);
			c->parent = t->parent;
			list_append(t->parent->children, list_iter_getval(it));
		} else 
			tree_delete_dealloc(list_iter_getval(it), mode, free_node);
	}

	list_free(t->children);
	if(t->parent) list_delete_item(t->parent->children, t);

	if(free_node) free_node(t->item);
	free(t);
}
