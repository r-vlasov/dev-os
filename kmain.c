#include "drv/drivers.h"

#include "include/memory_management/headers/paging.h"
extern void gdt_init();
extern void idt_init();
#include "include/heap/kmalloc.h"
extern uint32_t kmalloc(uint32_t);
#define MBOOT_MAGIC	0x2BADB002
/*	
 *	The fact is that according to MULTIBOOT SPEC. GRUB places in %eax a special identifier
 *	that tells us the kernel was loaded by compatible bootloader 		
*/



typedef uint32_t u32int;

struct multiboot
{
    u32int flags;
    u32int mem_lower;
    u32int mem_upper;
    u32int boot_device;
    u32int cmdline;
    u32int mods_count;
    u32int mods_addr;
    u32int num;
    u32int size;
    u32int addr;
    u32int shndx;
    u32int mmap_length;
    u32int mmap_addr;
    u32int drives_length;
    u32int drives_addr;
    u32int config_table;
    u32int boot_loader_name;
    u32int apm_table;
    u32int vbe_control_info;
    u32int vbe_mode_info;
    u32int vbe_mode;
    u32int vbe_interface_seg;
    u32int vbe_interface_off;
    u32int vbe_interface_len;
}  __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;


extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;

uint32_t initial_esp;
void kmain(multiboot_header_t *s, uint32_t initial_stack)
{
    initial_esp = initial_stack;
	gdt_init();
	idt_init();
	drivers_init();
	paging_init();
//	task_init();
	
//	current_directory = clone_directory(kernel_directory);
//	tty_write_string(current_directory);
	
//	tty_write_address(memcmp(current_directory, kernel_directory, sizeof(page_directory_t)));
	while(1);
}


