#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

#include "../../../lib/headers/stdlib.h"

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
	void		queue_set_head(queue_t* queue, void* value);
	void		queue_push(queue_t* queue, void* value);
	queue_node_t*	queue_pop(queue_t* queue);



#endif
