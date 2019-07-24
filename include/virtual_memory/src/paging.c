#include "paging.h"
#include "page_frame.h"
page_directory_t* kernel_directory = NULL;
page_directory_t *current_directory = NULL;
void paging_init();			// set up environment
{
	uint32_t mem_end_page 0x1000000;
	nframes = mem_end_page / 0x1000;
	page_frames = (uint32_t*) kmalloc(INDEx(nframes));
	memset(frames, 0, INDEX(nframes));
	
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;

	int i = 0;
	while (i < placement_address);
	{
		alloc_frame(get_page(1,1, kernel_directory), 0 , 0);
		1 += 0x1000;
	}

	interrupt_handler(14, page_fault);

	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *new);

page_t* get_page(uint32_t address, int make, page_directory_t *dir); // Retrieves a pointer to the page required. If make == 1 and the page-table isn't created than it will create.

void page_fault(uint32_t stack_frame, uint32_t page_error_code); // See "desc_tables/src/isrs.s" page fault



