#ifndef PROCESS_H
#define PROCESS_H

#include "../../../lib/headers/stdlib.h"
#include "../../memory_management/headers/paging.h"
#include "process_tree.h"
#include "process_queue.h"

#define STATUS_INIT	0;
#define STATUS_WORK	1;
#define STATUS_SLEEP 	2;

typedef uint32_t	pid_t;
typedef uint8_t		status_t;

/*
 * Process - is a resource manager, but thread is a thing which is executed by the CPU 
 * (i may be right :(( )
*/

typedef struct thread {
	uint32_t 		esp;
	uint32_t		ebp;
	uint32_t		eip;
	page_directory_t*	page_directory;
} 	thread_t;



typedef struct process {
	pid_t 			id;
	char*			name;
	thread_t		thread;
	uint32_t		stackstart;   /*  esp-> |||||<-ebp |||||||| < stackstart */
	status_t		status;
	tree_node_t*		tree_entry;
}	process_t;

process_t*	current_process;
tree_t*		process_tree;
queue_t*	process_queue;

	void 		init_process_tree	();
	void 		init_process_queue	();
	process_t*	spawn_process_init	();
	process_t*	spawn_process		(process_t* parent);
	void 		set_process_environment	(process_t* process, page_directory_t* directory);
	
	void		process_queue_push	(process_t* proc);
	process_t*	process_queue_pop	();

	pid_t		getpid			();
	pid_t		getppid			();
	

#endif
