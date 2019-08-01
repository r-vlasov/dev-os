#include "lib/headers/stdlib.h"
#include "include/memory_management/headers/paging.h"
typedef uint32_t pid_t;


typedef struct reg_state {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
}	Registers;

struct task_struct {
	uint32_t state;
	uint32_t priority;
	pid_t id;
	pid_t father;
	page_directory_t* page_dir;
	uint32_t start_stack;
	uint32_t stack_size;
	uint32_t eip;
	uint32_t esp;
	uint32_t ebp;
	Registers regs;
};
struct task_struct *tasks[10];
struct task_struct *last_task;

uint32_t task_init()
{
	//asm volatile("cli");
	last_task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
	*tasks[0] = *last_task;
	last_task->id = 0;
	last_task->father = -1;
	last_task->eip = 0;
	last_task->esp = 0;
	last_task->ebp = 0;
	//asm volatile("sti");
}

extern int read_eip();

//extern void tty_write_address(uint32);
uint32_t fork()
{
	//asm volatile("cli");
	struct task_struct* parent_task = (struct task_struct*) last_task;
	page_directory_t *directory = kernel_directory;
//	 = clone_directory(current_directory);
	
	struct task_struct* new_task = (struct task_struct*) kmalloc(sizeof(struct task_struct));
	new_task->id = 1;
	new_task->father = parent_task->id;
	new_task->page_dir = directory;
	tasks[1] = new_task;
	uint32_t eip = read_eip();
	if (last_task == parent_task)
	{
		uint32_t esp;
		asm volatile ("mov %%esp, %0" : "=r"(esp));
		uint32_t ebp;
		asm volatile ("mov %%ebp, %0" : "=r"(ebp));

		tasks[1]->esp = esp;
		tasks[1]->ebp = ebp;
		tasks[1]->eip = eip;
		tasks[1]->id = 1;
		tty_write_address(2);
		return 1;
	}
	else
	{
		tty_write_address(0);
		return 0;
	}
}
void switch_task()
{
    // If we haven't initialised tasking yet, just return.
    // Read esp, ebp now for saving later on.
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    // Read the instruction pointer. We do some cunning logic here:
    // One of two things could have happened when this function exits - 
    //   (a) We called the function and it returned the EIP as requested.
    //   (b) We have just switched tasks, and because the saved EIP is essentially
    //       the instruction after read_eip(), it will seem as if read_eip has just
    //       returned.
    // In the second case we need to return immediately. To detect it we put a dummy
    // value in EAX further down at the end of this function. As C returns values in EAX,
    // it will look like the return value is this dummy value! (0x12345).
    eip = read_eip();

    // Have we just switched tasks?
    if (eip == 0x12345)
        return;

    // No, we didn't switch tasks. Let's save some register values and switch.
    last_task->eip = eip;
    last_task->esp = esp;
    last_task->ebp = ebp;
    
    // Get the next task to run.
    last_task = tasks[1];
    // If we fell off the end of the linked list start again at the beginning.
    eip = last_task->eip;
    esp = last_task->esp;
    ebp = last_task->ebp;

    // Make sure the memory manager knows we've changed page directory.
//    current_directory = last_task->page_dir;
    // Here we:
    // * Stop interrupts so we don't get interrupted.
    // * Temporarily puts the new EIP location in ECX.
    // * Loads the stack and base pointers from the new task struct.
    // * Changes page directory to the physical address (physicalAddr) of the new directory.
    // * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    //   switched task.
    // * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    //   the next instruction.
    // * Jumps to the location in ECX (remember we put the new EIP in there).
    asm volatile("        	\
      cli;  \n                	\
      movl %0, %%ecx; \n      	\
      movl %1, %%esp; \n      	\
      movl %2, %%ebp; \n      	\
      movl %3, %%cr3; \n	\
      movl $0x12345, %%eax; \n 	\
      sti;                 \n   \
      jmp *%%ecx" : : "r"(eip), "r"(esp), "r"(ebp), "r"(kernel_directory->physicalAddress));
   
}


