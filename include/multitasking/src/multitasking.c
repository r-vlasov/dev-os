#include "../headers/multitasking.h"
#include "../headers/process.h"
#include "../headers/process_tree.h"
#include "../headers/process_stack.h"
#include "../headers/process_queue.h"
#include "../../../lib/headers/stdlib.h"


extern	process_t*		current_process;
extern  page_directory_t*	current_directory;
extern 	queue_t*		process_queue;

void		task_init()
{
	asm volatile ("cli");

	// initializing management structures
	init_process_tree();
	init_process_queue();
	//
	current_process = spawn_process_init();
	set_process_environment(current_process, current_directory);
	SWITCH_PAGE_DIRECTORY(current_process->thread.page_directory);
	asm volatile ("sti");
}

extern uint32_t __eip(); // external from assembly code:)


pid_t 		fork()
{
	asm volatile("cli");
	uint32_t _esp, _ebp, _eip;

	process_t* parent = current_process;
	page_directory_t* dir = clone_directory(current_directory);
	process_t* new_proc = spawn_process(current_process);
	set_process_environment(new_proc, dir);
	
	_eip = __eip();

	if (current_process == parent) 
	{

		asm volatile ("mov %%esp, %0" : "=r"(_esp));
		asm volatile ("mov %%ebp, %0" : "=r"(_ebp));

		copy_stack(new_proc->stackstart, parent->stackstart, 0x2000);

		new_proc->thread.esp = ( _esp + new_proc->stackstart ) - current_process->stackstart;
		new_proc->thread.ebp = ( _ebp + new_proc->stackstart ) - current_process->stackstart;
		new_proc->thread.eip = _eip;
		new_proc->status = STATUS_SLEEP;	


		// making new process ready
		process_queue_push(new_proc);
	
		asm volatile("sti");
		return new_proc->id;
	}
	else
	{
		asm volatile("sti");
		return 0;
	}
}

void switch_task() 
{
	if (!current_process)
	{
		return;	
	}

	if (process_queue->head == NULL || process_queue->tail == NULL || process_queue->nodes == 0)
	{
		return;
	}

	uint32_t _eip, _ebp, _esp;	
	asm volatile ("mov %%esp, %0" : "=r"(_esp));
	asm volatile ("mov %%ebp, %0" : "=r"(_ebp));
	_eip = __eip();
	if (_eip == 0x12345)
	{
		tty_write_string("Finish switching");
		return;
	}
	
	current_process->thread.esp = _esp;
	current_process->thread.ebp = _ebp;
	current_process->thread.eip = _eip;

	// switching process
	current_process = process_queue_pop();

	
	_eip = current_process->thread.eip;
	_ebp = current_process->thread.ebp;
	_esp = current_process->thread.esp;	
	
	current_directory = current_process->thread.page_directory;

	SWITCH_PAGE_DIRECTORY(current_directory);
	
	set_kernel_stack(current_process->stackstart);

	asm volatile ( " \
		cli\n \
		mov %0, %%ecx\n \
		mov %1, %%ebp\n \
		mov %2, %%esp\n \
		mov %3, %%cr3\n \
		mov $0x12345, %%eax\n \
		sti\n \
		jmp %%ecx " :: "r"(_eip), "r"(_ebp), "r"(_esp), "r"(current_directory->physicalAddress));
}

