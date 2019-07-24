#ifndef PAGING
#define PAGING

/* Paging from jamesmolloy.co.uk :)))) */

#define __PAGES_IN_PAGETABLE_LINE	1024

#include "../../../lib/headers/stdlib.h"

/*			Struct of page entries
 *
 *
 *
 *   | Frame address | AVAIL | RSVD | D | A | RSVD | U/S | R/W | P |
 *   31		   12 11    9 8    7  6   5  4    3   2     1    0	
 *
 * Fields:
 * P	-	present in the memory
 * R/W	-	read-only/writable
 * U/S	-	user-mode page
 * RSVD	-	reserved
 * A	-	?????????????????????????????????????????
 * D	-	?????????????????????????????????????????
 * AVAIL-	for kernel
 * Frame address -	The high 20bit of the frame address in physical memory
*/



/*	Bit Fields allow the packing of data in a structure.	*/

typedef struct {
	int present: 	1;
	int rw: 	1;
	int user: 	1;
	int accessed: 	1;
	int dirty : 	1;
	int unused:	7;
	int frame:	20; 		// frame will be __NOFRAME if we free page frame
} __attribute__((packed)) page_t;



typedef struct {
	page_t pages[__PAGES_IN_PAGETABLE_LINE];
} page_table_t;


/*
 * Intel proposes to use a two-tier system: The cpu reports the address of a 4kb page directory,
 * each entry in which contains the address of a 4 kb page table
*/

typedef struct {
	page_table_t *tables[1024];	// array of pointers to pagetables
	uint32_t tablesPhysical[1024];	// array of physical pagetables addresses
	uint32_t physicalAddress; 	// physical address of tablesPhysical
} page_directory_t;


/* Functions */

	void paging_init();			// set up environment

	page_t* get_page(uint32_t address, int make, page_directory_t *dir); // Retrieves a pointer to the page required. If make == 1 and the page-table isn't created than it will create.

	void page_fault(uint32_t stack_frame, uint32_t page_error_code); // See "desc_tables/src/isrs.s" page fault


#endif
