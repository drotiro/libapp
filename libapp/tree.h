#ifndef APP_TREE_H
#define APP_TREE_H

#include <libapp/list.h>

/*
 * libapp: an utility library to simplify
 * common tasks in app development.
 * (C) 2010-2013 - Domenico Rotiroti
 * Licensed under the LGPL 3.0
 *
 * tree.h: basic n-ary tree
 */

typedef struct tree_node_t tree;
typedef void (*tree_deallocator)(void * data);

typedef enum { APPEND_TO_PARENT, DEL_SUBTREE } tree_delete_mode;

tree *    tree_new(void * val);

tree *    tree_get_parent(tree * t);
list_iter tree_get_children(tree * t);
void *    tree_getval(tree * t);

tree *    tree_insert(tree * parent, void * item);

void      tree_delete(tree *t, tree_delete_mode mode);
void      tree_delete_dealloc(tree *t, tree_delete_mode mode, tree_deallocator free_node);

#endif //APP_TREE_H
