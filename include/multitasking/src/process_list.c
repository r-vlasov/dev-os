#include "../headers/process_list.h"

/*
typedef struct list_node {
	void*			value;
	struct 			*prev;
}	list_node_t;


typedef struct list {
	size_t			nodes;
	list_node_t		*head;
}	list_t;



	list_t*			list_create();
	list_node_t*		list_node_create(void* value);
	void			list_set_head(list_t* list, void* value);
	void 			list_add(list_t* list, void* value);

*/


/* */
list_t*		list_create()
{
	list_t* res = kmalloc(sizeof(list_t));
	res->head = NULL;
	res->nodes = 0;
	return res;
}


/* */
list_node_t*	list_node_create(void* value)
{
	list_node_t* node = kmalloc(sizeof(list_node_t));
	node->value = value;
	node->prev = NULL;
	return node;
}


/* */
void		list_set_head(list_t* list, void* value)
{
	list_node_t* lhead = list_node_create(value);
	list->head = lhead;
	list->head->prev = NULL;
	list->nodes = 1;
}


/* */
void		list_add(list_t* list, void* value)
{
	if (list->nodes)
	{
		list_node_t* node = list_node_create(value);
		list_node_t* i;
		for (i = list->head; i->prev != NULL; i = i->prev);
		i->prev = node;
		list->nodes++;
	}
	else
	{
		list_set_head(list, value);
	}
}
