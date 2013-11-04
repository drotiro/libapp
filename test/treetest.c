#include "libapp/tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_del(void * item)
{
	if(item) printf("deleting item %s\n", (char*) item);
}

void main(int argc, char* argv[])
{
	tree * t = tree_new("root");
	tree * p1, *p2;
	list_iter it;
	
	p1 = tree_insert(t, "one");
	p2 = tree_insert(t, "two");
	tree_insert(p1, "2nd level A");
	tree_insert(p2, "2nd level B1");
	tree_insert(p2, "2nd level B2");
	
	printf("Root node is: %s\n", (char*)tree_getval(t));
	printf("1st level:\n");
	for(it = tree_get_children(t); it; it = list_iter_next(it))
	{
		printf("(%s)\n", (char*)tree_getval(list_iter_getval(it)));
	}
	tree_delete(p2, APPEND_TO_PARENT);
	printf("1st level - after append to parent:\n");
	for(it = tree_get_children(t); it; it = list_iter_next(it))
	{
		printf("(%s)\n", (char*)tree_getval(list_iter_getval(it)));
	}

	tree_delete_dealloc(t, DEL_SUBTREE, check_del);
}
