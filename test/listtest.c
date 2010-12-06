#include "../list.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void main(int argc, char* argv[])
{
	list * l;
	list_iter it;
	int i;
	
	//list test
	l = list_new();
	for (i=0; i<10; ++i) {
		(i%2 ? list_append(l, i): list_prepend(l, i));
	}
	list_insert_at(l,20,9);
	list_append(l, 22);
	list_delete_item(l,22);
	list_delete_at(l, 10);
	printf("list size is %d\n", list_size(l));
	it = list_get_iter(l);
	while(it) {
		printf("data: %d\n", (int)list_iter_getval(it));
		it = list_iter_next(it);
	}
	list_free(l);
	
}
