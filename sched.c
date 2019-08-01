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
	uint32_t start_stack;
	uint32_t stack_size;
	Registers regs;
};

struct task_struct *tasks[NTASK];
struct task_struct *last_task;

uint32_t task_init()
{
	asm volatile("cli");
	last_task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
	tasks[0] = last_task;
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
	asm volatile("sti");
};


/* EIP register is stored on the stack and is retrieved from the stack by instructions like call, ret, iret */

//#define GET_EIP		asm volatile ("	pop %eax \n jmp %rax ");


extern uint32_t GET_EIP();
//////////////////////////////////////////////////////////////////////////////


uint32_t _ct = 0;

/*
void copy_stack(uint32_t new_stack_start, uint32_t size)
{

  	// Flush the TLB by reading and writing the page directory address again.
  	u32int pd_addr;
  	asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
  	asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

  	// Old ESP and EBP, read from registers.
  	u32int old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  	u32int old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

  	// Offset to add to old stack addresses to get a new stack address.
  	u32int offset            = (u32int)new_stack_start - initial_esp;

  	// New ESP and EBP.
  	u32int new_stack_pointer = old_stack_pointer + offset;
  	u32int new_base_pointer  = old_base_pointer  + offset;

  	// Copy the stack.
  	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  	// Backtrace through the original stack, copying new values into
  	// the new stack.  
  	for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  	{
    		u32int tmp = * (u32int*)i;
    		// If the value of tmp is inside the range of the old stack, assume it is a base pointer
    		// and remap it. This will unfortunately remap ANY value in this range, whether they are
    		// base pointers or not.
    		if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    		{
      			tmp = tmp + offset;
      			u32int *tmp2 = (u32int*)i;
      			*tmp2 = tmp;
    		}
	}
  }
*/

uint32_t fork()
{
	asm volatile("cli");
	struct task_struct* parent_task = (struct task_struct*) last_task;
//	page_directory_t *directory = clone_directory(current_directory);
	
	struct task_struct* new_task = (struct task_struct*) kmalloc(sizeof(struct task_struct));
	new_task->id = 1;
	new_task->father = parent_task->id;
	new_task->page_dir = current_directory;
	tasks[1] = new_task;
	uint32_t eip = GET_EIP();

	uint32_t esp;
	uint32_t ebp;
	asm volatile ("mov %%esp, %0" : "=r"(esp));
	asm volatile ("mov %%ebp, %0" : "=r"(ebp));
	
	if (last_task == parent_task)
	{
//		uint32_t esp;
//		uint32_t ebp;

//		asm volatile ("mov %%esp, %0" : "=r"(esp));
//		asm volatile ("mov %%ebp, %0" : "=r"(ebp));
		
//		for (int i = 0xE0000000 - 2 * 0x1000; i <= 0xE0000000; i += 0x1000)
//		{
//			alloc_frame(get_page(i, 1, new_task->page_dir), 0, 1);
//		}
		tasks[1]->regs.esp = esp;
		tasks[1]->regs.ebp = ebp;
		tasks[1]->regs.eip = eip;
//		tty_write_address(1);
		asm volatile("sti");
		return 1;
	}
	else
	{
//		asm volatile("sti");
//		tty_write_address(0);
		return 0;
	}
}
void switch_task() 
{
	if(!last_task)
		return;
    // If we haven't initialised tasking yet, just return.
    // Read esp, ebp now for saving later on.
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    // Read the instruction pointer. We do some cunning logic here:
    // One of two things could have happened when this function exits - 
    //   (a) We called the function and it returned the EIP as requested.
    //
    //   (b) We have just switched tasks, and because the saved EIP is essentially
    //       the instruction after read_eip(), it will seem as if read_eip has just
    //       returned.
    // In the second case we need to return immediately. To detect it we put a dummy
    // value in EAX further down at the end of this function. As C returns values in EAX,
    // it will look like the return value is this dummy value! (0x12345).
    eip = GET_EIP();
    // Have we just switched tasks?
    if (eip == 0x12345)
        return;

    // No, we didn't switch tasks. Let's save some register values and switch.
    last_task->regs.eip = eip;
    last_task->regs.esp = esp;
    last_task->regs.ebp = ebp;
    
    // Get the next task to run.
    last_task = tasks[1];
    // If we fell off the end of the linked list start again at the beginning.
    eip = last_task->regs.eip;
    esp = last_task->regs.esp;
    ebp = last_task->regs.ebp;

    // Make sure the memory manager knows we've changed page directory.
  //  current_directory = last_task->page_dir;
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
      jmp *%%ecx" : : "r"(eip), "r"(esp), "r"(ebp), "r"(&current_directory->tablesPhysical));
}


