




#include "../headers/syscall.h"
#include "../../registers.h"
#include "../../desc_tables/headers/interrupt_desc_table.h"
void syscall_handler(registers_t *regs);

void tty_out_char(char);
void tty_write_address(unsigned long);
void tty_write_string(const char*);

void __sys(uint32_t addr)
{
	asm volatile (" mov %0, %%eax " :: "r"(addr));
	asm volatile ("	int $128\n");
}


void syscall_write_string_handler(registers_t regs)	
{
	tty_write_address(regs.eax);
	asm volatile ("	sti");
}

