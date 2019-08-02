#include "kmalloc.h"

#include "../../drv/teletype/headers/tty.h"

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
	if (heap0)
	{
		void *addr;
		if (align)
		{
			addr = dmalloc_align(sz, heap0);
		}
		else
		{
			addr = dmalloc(sz, heap0);
		}
		if (phys != NULL)
		{
			page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
			*phys = (page->frame*PAGE_SIZE + ((uint32_t)addr & (0xFFF)));
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
	dfree(ptr, heap0);
}
