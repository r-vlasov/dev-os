#ifndef STRUCT_PROCESS_H
#define STRUCT_PROCESS_H

#include "../../memorymanagement/headers/paging.h"

#define TASK_RUNNING  			0
#define TASK_INTERRUPTIBLE		1
#define TASK_UNINTERRUPTIBLE		2
#define TASK_ZOMBIE			4
#define TASK_STOPPED			8


typedef unsigned long pid_t;

struct cpustate {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;

	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;


	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed));


struct task_struct {
	uint32_t		state;
	char			task_name[8];
	pit_t			pid_;	// proc number
	pid_t			ppid_;
	struct cpustate		cpustate_;

}

struct task_struct* current_task;

#define STACK_START 0xE0000000;
#define STACK_SIZE  0x1000;
void create_task_stack(void* start, uint32_t sz)
{
	for(uint32_t i = (uint32_t)start, i >= (uint32_t)start; i-= PAGE_SIZE)
	{
		alloc_frame ( get_page(i, 1, kernel_directory), 0, 0 );
	}
}

uint32_t id = 0;

struct task_struct* create_task()
{
	asm ("cli");
	struct task_struct* _task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
	_task->pid_t = id;
	_task->state = TASK_RUNNING;
	_task->cpustate_.eax = 0;
	_task->cpustate_.ebx = 0;
	_task->cpustate_.ecx = 0;
	_task->cpustate_.edx = 0;

	create_task_stack(STACK_START + id * STACK_SIZE, STACK_SIZE);

	
	_task->cpustate_.edi = 0;
	_task->cpustate_.esi = 0;

	_task->cpustate_.ebp = STACK_START + id*STACK_SIZE;
	_task->cpustate_.esp = STACK_START + id*STACK_SIZE;
	return _task;
}

void task_init()
{
	current_task = create_task();
	current_task->name = "init\0";
	asm volatile ("mov %0, %%esp" : "=r"(current_task->cpustate_.esp));
	asm volatile ("mov %0, %%ebp" : "=r"(current_task->cpustate_.ebp));
	id++;
	tty_write_string("hello");
}

struct cpustate* scheduler(struct cpustate* _cpustate)
{
	
}

	
