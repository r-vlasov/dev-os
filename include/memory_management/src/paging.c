#include "../headers/paging.h"
#include "../headers/page_frame.h"
#include "../../heap/kmalloc.h"
#include "../../../lib/headers/stdlib.h"

/*	VMM has 3 parts:
 *		- the page directory (page_directory_t)
 *		- the page table (page_table_t)
 *		- the actual page frame (page_t)
 *	It's activated by setting the bit PG = 1 in the CR0
 *	CR2 stores the linear address of the fauld
 *	CR3 strores the physical address of the page directory;
 *
 *
 *	----------------------------------------------------------
 *	Page directory contains of 1024 32-bit lines(PAGE DIRECTORY)(4KB).
 *	Each row contains 20 high-order bits of the next-level table address.
 *	----------------------------------------------------------
 *	Each page table has 1024 lines (PAGE TABLE).
 *	----------------------------------------------------------
 *	The physical address is obtained from the address of the page taken from the table and 
 *	the lower 12 bits of the linear address
*/

extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;
extern uint32_t placement_address;   			// Space after kernel_end(initial in linker script 
extern void idt_handler(uint8_t, void*, uint8_t);	// INT14 handler
extern void load_page_directory();			// load CR0, CR3 ( turn on paging:) )



#define INIT_PAGING(dir)			current_directory = dir; \
						asm volatile(	"pushl	%eax\n");\
						asm volatile(	"movl	%0, %%cr3\n" ::"r"(current_directory->physicalAddress));\
						asm volatile(	"movl 	%cr0, %eax\n");\
						asm volatile(	"orl	$0x80000000, %eax\n");\
						asm volatile(	"movl 	%eax, %cr0\n");\
						asm volatile( 	"popl	%eax");	

#define SWITCH_PAGE_DIRECTORY(dir)		asm volatile("	movl	%0, %%cr3\n" ::"r"(dir->physicalAddress));


#define DISABLE_PAGING()			asm volatile(	"pushf \n push 	%edx\n 		mov %cr0, %edx\n");\
						asm volatile( 	"and 	$0x7FFFFFFF, %edx\n");\
						asm volatile( 	"mov 	%edx, %cr0\n");

								
#define ENABLE_PAGING()				asm volatile(	"mov %cr0, %edx \n orl $(1 << 31), %edx \n mov %edx, %cr0");\
						asm volatile(	"pop %ecx \n pop %ebx \n pop %edx \n popf \n ");



// Kernel space | page frame | page dir //


void paging_init()
{
	uint32_t mem_end_page = 0xFFFFF000;  // The size of our memory

    	nframes = mem_end_page / PAGE_SIZE;	// Number of page frames
    	page_frames = (uint32_t*)kmalloc(INDEX(nframes));
    	memset(page_frames, 0, INDEX(nframes));

    	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    	memset(kernel_directory, 0, sizeof(page_directory_t));
	kernel_directory->physicalAddress = (uint32_t)kernel_directory->tablesPhysical;
	current_directory = kernel_directory;
    	int i;
	// there is because get_page changes placement_address
	
	for (i = KHEAP_START; i <= KHEAP_START + KHEAP_START_SIZE; i += PAGE_SIZE)
	{
		get_page(i, 1, kernel_directory);
	}
	
	
	/**
     	** Теперь нам необходимо тождественно отобразить
    	** адреса виртуальной памяти на адреса физической памяти,
    	** чтобы мы могли прозрачно обращаться к физической памяти,
     	** как будто страничная адресация не включена.
     	** Обратите внимание что мы специально используем здесь
     	** цикл while, т.к. внутри тела цикла значение переменной
     	** placement_address изменяется при вызове kmalloc().
     **/
    	i = 0;
    	while (i < placement_address)
    	{
        // Код ядра доступен для чтения но не для записи
        // из пространства пользователя
        	alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        	i += PAGE_SIZE;
    	}


	for(uint32_t s = KHEAP_START; s <= KHEAP_START + KHEAP_START_SIZE; s += PAGE_SIZE)
	{
	    	alloc_frame( get_page(s, 1, kernel_directory), 0, 0 );
	}
	
	// Initializing page fault handler
	idt_handler(14, page_fault, 0x8E);
	
	// Initial paging
	INIT_PAGING(kernel_directory);


	// Heap initialization
	heap_init(KHEAP_START, KHEAP_START_SIZE, KHEAP_MAX, 0, &heap0);


	current_directory = clone_directory(kernel_directory);
	current_directory = clone_directory(current_directory);
	SWITCH_PAGE_DIRECTORY(current_directory);
}


page_directory_t *clone_directory(page_directory_t *src)
{
	uint32_t phys;
	page_directory_t *dir = (page_directory_t*) kmalloc_ap(sizeof(page_directory_t), &phys);
	memset(dir, 0, sizeof(page_directory_t));

	dir->physicalAddress = phys + (uint32_t)dir->tablesPhysical - (uint32_t)dir;

	for (int i = 0; i < 1024; i++)
	{
		if (src->tables[i] == NULL)
			continue;
		if (src->tables[i] == kernel_directory->tables[i])
		{
			dir->tables[i] = src->tables[i];
			dir->tablesPhysical[i] = src->tablesPhysical[i];
			continue;
		}
		else
		{
			uint32_t phys2;
			dir->tables[i] = clone_table(src->tables[i], &phys2);
			dir->tablesPhysical[i] = phys2 | 0x07;
			continue;
		}
	}
	return dir;
}


static page_table_t* clone_table(page_table_t *src, uint32_t *physAddr)
{
	page_table_t* table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
	memset(table, 0, sizeof(page_table_t));

	for (int i = 0; i < 1024; i++)
	{
		if (src->pages[i].frame)
		{
			alloc_frame(&table->pages[i], 0, 0);
			if(src->pages[i].present)	table->pages[i].present = 1;
			if(src->pages[i].rw)		table->pages[i].rw = 1;
			if(src->pages[i].user)		table->pages[i].user = 1;
			if(src->pages[i].accessed)	table->pages[i].accessed = 1;
			if(src->pages[i].dirty)		table->pages[i].dirty = 1;
	
	
			asm volatile("cli");
			DISABLE_PAGING();
			asm volatile("	\n		\
				push %%ebx \n		\
				push %%ecx \n		\
				mov %0, %%ebx \n	\
				mov %1, %%ecx \n 	\
				mov $0x1000, %%edx \n	\
				loop:	\n		\
				mov (%%ebx), %%eax \n	\
				mov %%eax, (%%ecx) \n	\
				add $4, %%ebx \n	\
				add $4, %%ecx \n	\
				dec %%edx \n		\
				jnz loop" :: "r"(src->pages[i].frame * 0x1000), "r"(table->pages[i].frame * 0x1000));
			
			ENABLE_PAGING();
			asm volatile("sti");
		}
	}
	return table;
}


page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
       	uint32_t frame_index = address / (uint32_t) PAGE_SIZE;
	// Находим таблицу, содержащую адрес
	uint32_t table_index = frame_index / (uint32_t) 1024;
	if (dir->tables[table_index]) // Если таблица уже создана
		return &dir->tables[table_index]->pages[frame_index % (uint32_t)1024];
	else if(make)
	{
		uint32_t tmp; // Physical Address of dir.tablesPhysical
		dir->tables[table_index] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_index], 0, PAGE_SIZE);
		dir->tablesPhysical[table_index] = tmp | 0x7; // PRESENT, RW, US
		
		return &dir->tables[table_index]->pages[frame_index % (uint32_t)1024];
	}
	else
		return NULL;
}
void page_fault(registers_t regs)
{
	// CR2 stores the linear address of the fault
	uint32_t faulting_address;
	asm volatile ("mov %%cr2, %0" : "=r"(faulting_address));


	/*
	 * stack:	-> 	interrupt number
	 *  		->	interrupt error code		(see: IDT)
	 *  		-> 	............
	*/
	uint32_t err_code = regs.err_code;
	uint32_t present = !((err_code & 0x1));	// Page not present
	uint32_t rw = err_code & 0x2;		// Write operation ?
	uint32_t us = err_code & 0x4;		// Processor was in user-mode?
	uint32_t reserved = err_code & 0x8;	// Overwritten CPU reserved bits
	tty_write_string("page fault");

	if (present) tty_write_string("present");
	if (rw) tty_write_string("rw");	
	if (us) tty_write_string("us");
	if (reserved) tty_write_string("reserved");
	
	tty_write_address(faulting_address);
	while(1);
}




