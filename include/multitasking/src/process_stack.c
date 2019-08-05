#include "../headers/process_stack.h"


extern	void*			current_directory;


void move_stack(void *new_stack_start, void* old_stack_start, uint32_t size)
{
	uint32_t i;
	for (i = (uint32_t) new_stack_start; i >= (uint32_t) new_stack_start - size; i -= 0x1000)
	{
			alloc_frame(get_page(i, 1, current_directory), 0, 1);
	}


	uint32_t old_stack_pointer, old_base_pointer, offset;
	
	asm volatile("mov %%esp, %0" : "=r"(old_stack_pointer));
	asm volatile("mov %%ebp, %0" : "=r"(old_base_pointer));

	uint32_t init_esp = (uint32_t)old_stack_start;

	offset = (uint32_t)new_stack_start - init_esp;
	uint32_t new_stack_pointer = old_stack_pointer + offset;
	uint32_t new_base_pointer = old_base_pointer + offset;

	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, init_esp - old_stack_pointer);

	for (int i = (uint32_t) new_stack_start; i > (uint32_t) new_stack_start - size; i -= 4)
	{
		uint32_t tmp = *(uint32_t*)i;
		if ((old_stack_pointer < tmp) && (tmp < init_esp))
		{
			tmp = tmp + offset;
			uint32_t *tmp2 = (uint32_t*)i;
			*tmp2 = tmp;
		}
	}
	asm volatile("mov %0, %%esp" : : "r"(new_stack_pointer));
	asm volatile("mov %0, %%ebp" : : "r"(new_base_pointer));
}



void copy_stack(void *new_stack_start, void* old_stack_start, uint32_t size)
{
	uint32_t i;
	for (i = (uint32_t) new_stack_start; i >= (uint32_t) new_stack_start - size; i -= 0x1000)
	{
			alloc_frame(get_page(i, 1, current_directory), 0, 1);
	}

	uint32_t old_stack_pointer, old_base_pointer, offset;
	
	asm volatile("mov %%esp, %0" : "=r"(old_stack_pointer));
	asm volatile("mov %%ebp, %0" : "=r"(old_base_pointer));
	
	uint32_t init_esp = (uint32_t) old_stack_start;
	
	offset = (uint32_t)new_stack_start - init_esp;
	uint32_t new_stack_pointer = old_stack_pointer + offset;
	uint32_t new_base_pointer = old_base_pointer + offset;

	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, init_esp - old_stack_pointer);

	for (int i = (uint32_t) new_stack_start; i > (uint32_t) new_stack_start - size; i -= 4)
	{
		uint32_t tmp = *(uint32_t*)i;
		if ((old_stack_pointer < tmp) && (tmp < init_esp))
		{
			tmp = tmp + offset;
			uint32_t *tmp2 = (uint32_t*)i;
			*tmp2 = tmp;
		}
	}
}


	
