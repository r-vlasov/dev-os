#include "../headers/syscall.h"
#include "../../registers.h"
#include "../../desc_tables/headers/interrupt_desc_table.h"

extern void tty_write_address(unsigned long);



void syscall_write_address_handler(registers_t regs)	
{
	asm volatile (" push %0\n" :: "r"(regs.eax));
	asm volatile (" call tty_write_address\n");
	asm volatile (" pop %eax\n");
	asm volatile (" sti");
}

