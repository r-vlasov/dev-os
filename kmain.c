#include "drv/drivers.h"

#include "include/memory_management/headers/paging.h"
extern void gdt_init();
extern void idt_init();
extern void heap_init();
#include "include/heap/kmalloc.h"
extern uint32_t kmalloc(uint32_t);
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

	uint32_t a = find_chunk_align(0x20, heap);
	tty_write_address(a);
	while(1);
}


