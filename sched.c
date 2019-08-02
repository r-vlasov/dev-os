#include "lib/headers/stdlib.h"
#include "include/memory_management/headers/paging.h"
typedef uint32_t pid_t;

#define NTASK 64

#define TASK_RUNNING 0
#define TASK_STOPPED 1

typedef struct reg_state {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
}	Registers;

struct task_struct {
	uint32_t state;
	uint32_t priority;
	pid_t id;
	pid_t father;
	page_directory_t* page_dir;
	Registers regs;
};


/* EIP register is stored on the stack and is retrieved from the stack by instructions like call, ret, iret */
#define SETUP_LABEL_GET_EIP	asm("_geteip: mov (%esp), %eax \n ret \n");

uint32_t get_eip()
{
	asm volatile ("call _geteip \n"); 
}


///////////////////////////////




struct task_struct *tasks[6];
struct task_struct *last_task;

SETUP_LABEL_GET_EIP;

extern uint32_t GET_EIP();
uint32_t task_init()
{
	asm volatile("cli");
	last_task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
	last_task->id = 0;
	last_task->father = -1;
	last_task->regs.eax = 0;
	last_task->regs.ebx = 0;
	last_task->regs.ecx = 0;
	last_task->regs.edx = 0;
	last_task->regs.esi = 0;
	last_task->regs.edi = 0;
	last_task->regs.eip = 0;
	last_task->regs.esp = 0;
	last_task->regs.ebp = 0;
	last_task->regs.cr3 = 0;
	last_task->regs.eflags = 0;
	last_task->page_dir = current_directory;
	tasks[0] = last_task;
	asm volatile("sti");
	return 1;
};


uint32_t _ct = 0;

uint32_t fork()
{
	asm volatile("cli");
	struct task_struct* parent_task = (struct task_struct*) last_task;
	
	page_directory_t *dir = clone_directory(parent_task->page_dir);
	struct task_struct* new_task = (struct task_struct*) kmalloc(sizeof(struct task_struct));
	tasks[1]->id = 1;
	tasks[1]->father = parent_task->id;
	tasks[1]->page_dir = dir;
	
	uint32_t eip = GET_EIP();


	if (last_task == parent_task)
	{
		uint32_t esp;
		uint32_t ebp;

		asm volatile ("mov %%esp, %0" : "=r"(esp));
		asm volatile ("mov %%ebp, %0" : "=r"(ebp));
		
	
		tasks[1]->regs.esp = esp;
		tasks[1]->regs.ebp = ebp;
		tasks[1]->regs.eip = eip;
		asm volatile("sti");
		tty_write_string("I'm father");
		return 1;
	}
	else
	{
		tty_write_string("i'm children");
		return 0;
	}
}
void switch_task() 
{
	last_task = tasks[1];
	uint32_t eip, ebp, esp;
	eip = last_task->regs.eip;
	ebp = last_task->regs.ebp;
	esp = last_task->regs.esp;
	current_directory = last_task->page_dir;
	asm volatile ( " \
		cli\n \
		mov %0, %%ecx\n \
		mov %1, %%cr3\n \
		sti\n \
		jmp %%ecx " :: "r"(eip), "r"(current_directory->physicalAddress));

}
