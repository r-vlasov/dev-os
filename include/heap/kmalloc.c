#include "kmalloc.h"


uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
	if (heap)
	{
		return dmalloc (sz);
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
