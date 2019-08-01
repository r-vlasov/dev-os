#include "lib/headers/stdlib.h"

typedef uint32_t pid_t;


typedef struct reg_state {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
}	Registers;

struct task_struct {
	uint32_t state;
	uint32_t priority;
	pid_t id;
	pid_t father;
	uint32_t start_stack;
	uint32_t stack_size;
	Registers regs;

};


#define INIT_TASK_0 {0, 15, 0, -1, 0xE0000000}

struct task_struct *tasks[10];
struct task_struct *last_task;


union task_union {
	struct task_struct task;
	char stack[0x1000];
};

extern uint32_t kernel_directory;
uint32_t task_init()
{

	struct task_struct init_task = INIT_TASK_0;
	*tasks[0] = init_task;
	*last_task = init_task;
	for (int i = init_task.start_stack; i >= init_task.start_stack - init_task.stack_size; i -= 0x1000)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0, 1);
	}

}


