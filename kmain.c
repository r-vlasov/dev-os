#include "drv/drivers.h"

#include "include/memory_management/headers/paging.h"
extern void gdt_init();
extern void idt_init();
#include "include/heap/kmalloc.h"
#include "include/syscall/headers/syscall.h"

typedef uint32_t u32int;

typedef struct multiboot
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
} __attribute__((packed)) multiboot_header_t;


extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;

uint32_t initial_esp;
extern char initrd[];

void kmain(uint32_t initial_stack, multiboot_header_t* mboot)
{
	initial_esp = initial_stack;
	gdt_init();
	idt_init();
	drivers_init();
	paging_init();
	task_init();
	switch_to_user_mode();	

	SYSCALL_WRITE_ADDRESS(0x23);	

	while(1);
}


