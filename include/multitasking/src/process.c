#include "../headers/process.h"
#include "../headers/process_queue.h"
#include "../headers/process_tree.h"

extern tree_t*		process_tree;
extern queue_t*		process_queue;
extern process_t*	current_process;	// getpid, getppid
extern uint32_t		initial_esp;		// initializing

pid_t pids = 0;

/* */
void 	init_process_tree() 
{
	process_tree = tree_create();
}

void 	init_process_queue()
{
	process_queue = queue_create();
}


/* */
process_t*	spawn_process_init()
{
	if (!process_tree || !process_queue)
		return NULL;
	
	process_t* init = kmalloc(sizeof(process_t));
		
	/* Init - is a root process */
	tree_set_root(process_tree, (void*)init);
//	queue_set_head(process_queue, (void*)init);

	init->id = (pid_t)pids;
	init->status = STATUS_INIT;
	init->stackstart = initial_esp;
	init->tree_entry = process_tree->root;
	return init;
}

/* */
process_t*	spawn_process(process_t* parent)
{
	if (!parent)
	{
		tty_write_string("Can't spawning\n");
		return NULL;
	}

	process_t* process = kmalloc(sizeof(process_t));
	process->id = ++pids;
	process->status = STATUS_SLEEP;
	process->stackstart = (uint32_t)kmalloc(0x2000) + 0x2000;
//	tree_node_t* node = tree_node_create(process);
//	process->tree_entry = node;

// node -> process	
	tree_node_t* t = tree_node_insert_child(process_tree, parent->tree_entry, process);
	process->tree_entry = t;
	return process;
}

/* */
void		set_process_environment(process_t* proc, page_directory_t* directory)
{
	proc->thread.page_directory = directory;
}


/* */
void 		process_queue_push(process_t* proc)
{
	queue_push(process_queue, proc);
}


/* */
process_t*	process_queue_pop()
{
	queue_node_t*	res = queue_pop(process_queue);
	process_t* proc = (process_t*) res->value;
//	kfree(res);   <- troubles
	return proc;
}


/* */
pid_t		getpid()
{
	return current_process->id;
}


/* */
pid_t		getppid()
{
	if (current_process->tree_entry->parent)
	{
		return ((process_t*)(current_process->tree_entry->parent->value))->id;
	}
	else
	{
		return -1;
	}
}
