#include "drv/drivers.h"

#include "include/virtual_memory/headers/paging.h"
extern void gdt_init();
extern void idt_init();
extern void create_heap(uint32_t, uint32_t, uint32_t, uint8_t);
extern void* malloc1(size_t);
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
	
	tty_write_string("sad");
	create_heap(0xC0000000, 0x10000, 0x50000, 0);

	char* a = (char*)malloc1(8);
	uint32_t b = malloc1(8);
	uint32_t c = malloc1(8);

	tty_write_address(a);
	tty_out_char('\n');
	tty_write_address(b);
	tty_out_char('\n');
	tty_write_address(c);
	while(1);
}


