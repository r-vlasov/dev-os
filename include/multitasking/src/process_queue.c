#include "../headers/process_queue.h"
/*
typedef struct queue_node {
	void*			value;
	struct queue_node	*prev;
}	queue_node_t;

typedef struct queue {
	size_t			nodes;
	queue_node_t		*head;
	queue_node_t		*tail;
}	queue_t;

	

	queue_t*	queue_create();
	queue_node_t*	queue_node_create(void* value);
	void 		queue_set_head(queue_t* queue, void* value);
	void		queue_push(queue_t* queue, void* value);
	queue_node_t*	queue_pop(queue_t* queue);


*/

/* */
queue_t*	queue_create()
{
	queue_t* res = kmalloc(sizeof(queue_t));
	res->head = NULL;
	res->tail = NULL;
	res->nodes = 0;
	return res;
}

/* */
queue_node_t*	queue_node_create(void* value)
{
	queue_node_t* res = kmalloc(sizeof(queue_node_t));
	res->value = value;
	res->prev = NULL;
	return res;
}

/* */
void 		queue_set_head(queue_t* queue, void* value)
{
	queue_node_t* nhead = queue_node_create(value);
	queue->head = nhead;
	queue->head->prev = NULL;
	queue->tail = 0;
	queue->nodes = 1;
}


/* */
void 		queue_push(queue_t* queue, void* value)
{
	if (!queue->nodes)
	{
		queue_set_head(queue, value);
		return;
	}
	queue_node_t* res = queue_node_create(value);
	res->value = value;
	if (!queue->tail)
	{
		queue->tail = res;
		queue->head->prev = queue->tail;
	}
	else
	{
		queue->tail->prev = res;
		queue->tail = res;
	}
	queue->nodes++;
}


/* */
queue_node_t*		queue_pop(queue_t* queue)
{	
	queue_node_t* res = queue->head;
	if (queue->head == queue->tail)
	{
		queue->head = NULL;
		queue->tail = NULL;
		return res;
	}	
	else 
	{
		queue->head = queue->head->prev;
		return res;
	}
}
