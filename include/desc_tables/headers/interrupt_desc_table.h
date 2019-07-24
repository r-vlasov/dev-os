#ifndef INTERRUPT_DESC_TABLE_H
#define INTERRUPT_DESC_TABLE_H


#include "../../../lib/headers/stdlib.h"

typedef struct {
	uint16_t lowerbits;
	uint16_t selector;
	uint8_t  reserved;
	uint8_t  type;
	uint16_t higherbits;
} __attribute__((packed)) Int_Node;


	/* Interrupt Description table Register */
/* 
 * A special 48-bit register that describes the location and  
 * size of the table containing the descriptors of interrupt 
 * */
typedef struct {
	uint16_t limit;
	void*    base;
} __attribute__((packed)) IDTR;


uint8_t irq_base;

void idt_handler(uint8_t , void* , uint8_t); 	// int handler

void idt_init();				// int description table initial



#define IRQ_HANDLER(name) void name(); \
	asm(#name ": pusha \n call _" #name " \n movb $0x20, %al \n outb %al, $0x20 \n outb %al, $0xA0 \n popa \n iret"); \
	void _ ## name()


#endif

