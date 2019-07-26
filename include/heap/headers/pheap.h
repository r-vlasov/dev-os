#ifndef PHEAP_H
#define PHEAP_H

/* Like "placement new" */
/* from jamesmolloy.co.uk/tutorial_html */


/* This memory allocator will allocate data after kernel, but we can't clear this bytes */
/* It's not a problem, because there will be a page tables */


#include "../../../lib/headers/stdlib.h"


	uint32_t placement_address; //  start address (after kernel)


	uint32_t pmalloc_int(uint32_t sz, int align, uint32_t *phys);
	uint32_t pmalloc_a(uint32_t sz);
	uint32_t pmalloc_p(uint32_t sz, uint32_t *phys);
	uint32_t pmalloc_ap(uint32_t sz, uint32_t *phys);
	uint32_t pmalloc(uint32_t sz);



#endif

