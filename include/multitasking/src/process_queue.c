#include "../headers/process_queue.h"


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
	queue->tail = queue->head;
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
		queue->head = queue->tail;
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
	if (queue->nodes == 0)
		tty_write_string("Can't pop, queue has zero size");
	queue_node_t* res = queue->head;
	if (queue->head == queue->tail)
	{
		queue->head = NULL;
		queue->tail = NULL;
		queue->nodes = 0;
		return res;
	}	
	else 
	{
		queue->head = queue->head->prev;
		queue->nodes--;
		return res;
	}
}
