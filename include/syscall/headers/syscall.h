
#ifndef SYSCALL_H
#define SYSCALL_H

#include "../../registers.h"

extern void tty_write_address(unsigned long);


#define  SYSCALL_WRITE_ADDRESS(addr)	asm volatile (" push %eax");\
					asm volatile (" mov %0, %%eax\n" :: "r"(addr ));\
					asm volatile ("	int $128\n");\
					asm volatile (" pop %eax");



void syscall_write_address_handler(registers_t regs);

#endif
