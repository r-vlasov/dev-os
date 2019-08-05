#include "../headers/global_desc_table.h"
#include "../headers/tss.h"
#include "../../../lib/headers/stdlib.h"

#define N_GDT_ENTRIES	6

extern void load_gdt(void*);
extern void load_tss();


extern uint32_t initial_esp;
GDT_Node GDT[N_GDT_ENTRIES];
GDTR gdtr;
struct tss_entry_t tss_entry;



void gdt_init()
{
    	gdtr.limit = (sizeof(GDT_Node)*N_GDT_ENTRIES) - 1;
    	gdtr.base  = (uint32_t)&GDT;

    	gdt_set_gate(0, 0, 0, 0, 0);                // Нулевой сегмент
    	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);    // Сегмент кода
    	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);    // Сегмент данных
    	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);    // Сегмент кода уровня пользовательских процессов
    	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);    // Сегмент данных уровня пользовательских процессов

    	write_tss(5, 0x10, initial_esp);
	load_gdt(&gdtr);  // Loading gdt (asm function)
	load_tss();
}

void write_tss(uint32_t num, uint32_t ss0, uint32_t esp0)
{
	uint32_t base = (uint32_t)&tss_entry;
	uint32_t limit = (uint32_t)base + sizeof(struct tss_entry_t);

	gdt_set_gate(num, base, limit, 0xE9, 0x00);
	memset(&tss_entry, 0x0, sizeof(struct tss_entry_t));

	tss_entry.ss0 	= 	ss0;
	tss_entry.esp0	=	esp0;
	tss_entry.cs	=	0x0b;
	tss_entry.ss	=
		tss_entry.ds =
		tss_entry.es =
		tss_entry.fs =
		tss_entry.gs = 	0x13;
	tss_entry.iomap_base = sizeof(tss_entry);
}

void set_kernel_stack(uint32_t stack)
{
	tss_entry.esp0 = stack;
}

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    GDT[num].base_lowerbits     = (base & 0xFFFF);
    GDT[num].base_middlebits = (base >> 16) & 0xFF;
    GDT[num].base_higherbits     = (base >> 24) & 0xFF;

    GDT[num].limit_lowerbits     = (limit & 0xFFFF);
    GDT[num].granularity = (limit >> 16) & 0x0F;

    GDT[num].granularity |= gran & 0xF0;
    GDT[num].access = access;
}

