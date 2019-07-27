#include "kmalloc.h"



uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
	if (heap)
	{
		void *addr;
		addr = alloc(sz, heap);

		if (phys != 0)
		{
			page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
			*phys = page->frame*PAGE_SIZE + (((uint32_t)addr)& (-PAGE_SIZE));
		}
		return (uint32_t)addr;
	}
	else
	{
		return pmalloc_int(sz, align, phys);
	}
}

uint32_t kmalloc_a(uint32_t sz)
{
	return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
	return kmalloc_int(sz, 0, phys);
}


uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
	return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz)
{
	return kmalloc_int(sz, 0, 0);
}

void kfree(uint32_t ptr)
{
	if (heap)
	{
		dfree(ptr);
	}
}
