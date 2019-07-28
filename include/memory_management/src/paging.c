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
extern uint32_t placement_address;   			// Space after kernel_end(initial in linker script 
extern void idt_handler(uint8_t, void*, uint8_t);	// INT14 handler
extern void load_page_directory();			// load CR0, CR3 ( turn on paging:) )



#define SWITCH_PAGE_DIR(kernel_directory)	asm volatile(	"pushl	%eax\n");\
						asm volatile(	"movl	%0, %%cr3\n" ::"r"(&kernel_directory->tablesPhysical));\
						asm volatile(	"movl 	%cr0, %eax\n");\
						asm volatile(	"orl	$(1 << 31), %eax\n");\
						asm volatile(	"movl 	%eax, %cr0\n");\
						asm volatile( 	"popl	%eax");	




// Kernel space | page frame | page dir //


void paging_init()
{
	uint32_t mem_end_page = 0xFFFFF000;  // The size of our memory

    	nframes = mem_end_page / PAGE_SIZE;	// Number of page frames
    	page_frames = (uint32_t*)kmalloc(INDEX(nframes));
    	memset(page_frames, 0, INDEX(nframes));

    	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    	memset(kernel_directory, 0, sizeof(page_directory_t));

    	int i;
	// there is because get_page changes placement_address
	
	for (i = KHEAP_START; i <= KHEAP_START + KHEAP_START_SIZE; i += PAGE_SIZE)
	{
		get_page(i, 1, kernel_directory);
	}
	
	
	for (i = KHEAP2_START; i <= KHEAP2_START + KHEAP_START_SIZE; i += PAGE_SIZE)
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
        	alloc_frame( get_page(i, 1, kernel_directory),0,0);
        	i += PAGE_SIZE;
    	}


	// Initializing page fault handler
	idt_handler(14, page_fault, 0x8E);
	

	for(uint32_t s = KHEAP_START; s <= KHEAP_START + KHEAP_START_SIZE; s += PAGE_SIZE)
	{
	    	alloc_frame( get_page(s, 1, kernel_directory), 0, 0);
	}
	for(uint32_t s = KHEAP2_START; s <= KHEAP2_START + KHEAP_START_SIZE; s += PAGE_SIZE)
	{
	    	alloc_frame( get_page(s, 1, kernel_directory), 0, 0);
	}


	// Initial paging
	SWITCH_PAGE_DIR(kernel_directory);


	// Heap initialization
	heap_init(KHEAP_START, KHEAP_START_SIZE, KHEAP_MAX, 0, &heap0);
	heap_init(KHEAP2_START, KHEAP2_START_SIZE, KHEAP_MAX, 0, &heap1);


}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
       	uint32_t frame_index = address / (uint32_t) PAGE_SIZE;
	// Находим таблицу, содержащую адрес
	uint32_t table_index = frame_index / (uint32_t) __PAGES_IN_PAGETABLE_LINE;
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




