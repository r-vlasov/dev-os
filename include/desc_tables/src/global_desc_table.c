#include "../headers/global_desc_table.h"
#include "../../../lib/headers/stdlib.h"

extern void load_gdt(void*);

GDT_Node GDT[5];





struct TSS_STRUCT {

        unsigned int back_link;

        unsigned int esp0, ss0;

        unsigned int esp1, ss1;

        unsigned int esp2, ss2;

        unsigned int cr3;

        unsigned int eip;

        unsigned int eflags;

        unsigned int eax,ecx,edx,ebx;

        unsigned int esp, ebp;

        unsigned int esi, edi;

        unsigned int es, cs, ss, ds, fs, gs;

        unsigned int ldt;

        unsigned int trace_bitmap;

};


struct TSS_STRUCT tss;

GDTR gdtr;

void gdt_init()
{
    tss.esp0 = 0xE0000000;
    tss.ss0 = 0x10;
    gdtr.limit = (sizeof(GDT_Node)*5);
    gdtr.base  = (uint32_t)&GDT;

    gdt_set_gate(0, 0, 0, 0, 0);                // Нулевой сегмент
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);    // Сегмент кода
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);    // Сегмент данных
    gdt_set_gate(3, &tss, sizeof(struct TSS_STRUCT), 0x89, 0xCF);    // Сегмент кода уровня пользовательских процессов
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0x89, 0xCF);    // Сегмент данных уровня пользовательских процессов

    load_gdt(&gdtr);  // Loading gdt (asm function)
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

