#include "../headers/interrupt_desc_table.h"
#include "../../../drv/teletype/headers/tty.h"

#define IDT_SIZE 256

extern void outport(uint8_t, uint8_t);
extern void load_idt(void*);


/* from isrs.s - handler of 0-31 interrupts */
extern void __isr0();
extern void __isr1();
extern void __isr2();
extern void __isr3();
extern void __isr4();
extern void __isr5();
extern void __isr6();
extern void __isr7();
extern void __isr8();
extern void __isr9();
extern void __isr10();
extern void __isr11();
extern void __isr12();
extern void __isr13();
extern void __isr14();
extern void __isr15();
extern void __isr16();
extern void __isr17();
extern void __isr18();
extern void __isr19();
extern void __isr20();


/* Interrupt Descriptor table */
Int_Node idt[IDT_SIZE];

void idt_handler(uint8_t index, void* handler, uint8_t type)
{
	asm volatile("pushf\n cli"); //PUSHF and POPF saves the contents of the flags register
	uint32_t address = (unsigned long)handler;
	idt[index].selector = 0x08;
	idt[index].lowerbits = address & 0xFFFF;
	idt[index].higherbits = (address & 0xffff0000) >> 16;
	idt[index].type = type;
	idt[index].reserved = 0;
	asm volatile("popf\n sti");
}

void timer();


void idt_init()
{
	memset(idt, 0, 256 * sizeof(Int_Node));
	IDTR idtr = {256 * sizeof(Int_Node), idt};
	/* PIC (Programmable Interrupt Controller) settings */ 
	outport(0x20, 0x11);
	outport(0xA0, 0x11);
	outport(0x21, 0x20);
	outport(0xA1, 0x28);
	outport(0x21, 0x04);
	outport(0xA1, 0x02);
	outport(0x21, 0x01);
	outport(0xA1, 0x01);
	outport(0x21, 0x00);
	outport(0xA1, 0x00);
	idt_handler(0x20, (uint32_t)timer, 0x8E);


	/*	Initial software handlers of interrupts	    */
	idt_handler(0, (uint32_t)__isr0,   0x8E);	
	idt_handler(1, (uint32_t)__isr1,   0x8E);	
	idt_handler(2, (uint32_t)__isr2,   0x8E);
	idt_handler(3, (uint32_t)__isr3,   0x8E);	
	idt_handler(4, (uint32_t)__isr4,   0x8E);
	idt_handler(5, (uint32_t)__isr5,   0x8E);
	idt_handler(6, (uint32_t)__isr6,   0x8E);	
	idt_handler(7, (uint32_t)__isr7,   0x8E);	
	idt_handler(8, (uint32_t)__isr8,   0x8E);
	idt_handler(9, (uint32_t)__isr9,   0x8E);	
	idt_handler(10, (uint32_t)__isr10,  0x8E);
	idt_handler(11, (uint32_t)__isr11,  0x8E);
	idt_handler(12, (uint32_t)__isr12,  0x8E);	
	idt_handler(13, (uint32_t)__isr13,  0x8E);	
	idt_handler(14, (uint32_t)__isr14,  0x8E);
	idt_handler(15, (uint32_t)__isr15,  0x8E);	
	idt_handler(16, (uint32_t)__isr16,  0x8E);
	idt_handler(17, (uint32_t)__isr17,  0x8E);
	idt_handler(18, (uint32_t)__isr18,  0x8E);	

	load_idt(&idtr); // Loading the address of IDTR	

}


// Our assemler ISR_handler firstly pushs error code
// 			    secondly pushs number of interrupts
// We use calling convention __cdecl, so the first parameter is number and the second is error
void isr(registers_t regs)
{
	tty_out_char('0' + regs.number);
	asm volatile("sti");
}




extern void switch_task();

IRQ_HANDLER(timer){
	outport(0x20, 0x20);
	(*((char*)(0xB8000 + 79*2)))++;
	switch_task();  
}



