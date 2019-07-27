#ifndef REGISTERS_H
#define REGISTERS_H

#include "../lib/headers/stdlib.h"

typedef struct	{
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t number;
	uint32_t err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;


#endif
