#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H


#include "../../../lib/headers/stdlib.h"

typedef struct list_node {
	void*			value;
	struct list_node 	*prev;
}	list_node_t;


typedef struct list {
	size_t			nodes;
	list_node_t		*head;
}	list_t;



	list_t*			list_create();
	list_node_t*		list_node_create(void* value);
	void			list_set_head(list_t* list, void* value);
	void 			list_add(list_t* list, void* value);


#endif
