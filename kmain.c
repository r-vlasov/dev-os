#include "drv/drivers.h"
#include "include/virtual_memory/headers/paging.h"
extern void gdt_init();
extern void idt_init();

#define MBOOT_MAGIC	0x2BADB002
/*	
 *	The fact is that according to MULTIBOOT SPEC. GRUB places in %eax a special identifier
 *	that tells us the kernel was loaded by compatible bootloader 		
*/

void kmain(int magic, struct multiboot *multiboot_specification)
{
	if (magic != MBOOT_MAGIC)
	{
		return;
	}
	gdt_init();
	idt_init();
	drivers_init();
	paging_init();
	uint32_t * ptr = (uint32_t*) 0xa000000;
	uint32_t do2 = *ptr;
	while(1);
}


