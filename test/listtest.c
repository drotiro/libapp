#include "libapp/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void list_dump(list *l, const char * fmt)
{
	list_iter it = list_get_iter(l);
	printf("list size is %d\n", list_size(l));
	while(it) {
		printf(fmt, list_iter_getval(it));
		it = list_iter_next(it);
	}	
}

void main(int argc, char* argv[])
{
	list * l;
	list_iter it;
	int i;
	char* sv[] = { "fff", "zzz", "rrr", "bbb", "ggg", "hhh", "lll", "ttt" };
	
	//list test
	l = list_new();
	for (i=0; i<10; ++i) {
		(i%2 ? list_append(l, (void*)i): list_prepend(l, (void*)i));
	}
	list_insert_at(l, (void*)20,9);
	list_append(l, (void*)22);
	list_delete_item(l, (void*)22);
	list_delete_at(l, 10);
	list_dump(l, "%d\n");
	list_free(l);
	
	//string list test
	l = list_new_full(free);
	for(i=0; i<8; ++i) list_append(l, strdup(sv[i]));
	list_prepend(l, strdup("nine"));
	list_delete_item_comp(l, strdup("seven"), (list_comparator)strcmp);
	list_delete_item_comp(l, "eee", (list_comparator)strcmp);
	list_dump(l, "%s\n");
	list_free(l);
	
	l = list_new();
	for(i=0; i<8; ++i) list_insert_sorted_comp(l, sv[i], (list_comparator)strcmp);
	list_dump(l, "%s\n");
	list_free(l);
}
