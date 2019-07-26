#include "drv/drivers.h"

#include "include/virtual_memory/headers/paging.h"
extern void gdt_init();
extern void idt_init();
extern void heap_init();
extern void* kmalloc(size_t);
extern void kfree(void*);
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
	heap_init();
	int* a = kmalloc(8);
	int* b = kmalloc(1);
	int* c = kmalloc(1);
	kfree(b);
	int* d = kmalloc(1);

	tty_write_address(a);
	tty_out_char('\n');
	tty_write_address(b);
	tty_out_char('\n');
	tty_write_address(c);
	tty_out_char('\n');
	tty_write_address(d);
	while(1);
}


