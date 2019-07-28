#include "kmalloc.h"

#include "../../drv/teletype/headers/tty.h"

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
	if (heap)
	{
		void *addr;
		addr = dmalloc(sz);
		if (phys != 0)
		{
			page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
			tty_out_char("A"); tty_write_address(page); tty_out_char("\n");
			*phys = page->frame*0x1000 + (uint32_t)addr & (- 0x1000);
			tty_out_char("C"); tty_write_address(page); tty_out_char("\n");
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
