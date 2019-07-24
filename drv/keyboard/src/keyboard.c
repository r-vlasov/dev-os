#include "../headers/keyboard.h"
#include "../headers/keyboard_map.h"
#include "../../../include/desc_tables/headers/interrupt_desc_table.h"
#include "../../teletype/headers/tty.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C


extern char inport(uint8_t);
extern void outport(uint8_t, uint8_t);

void keyboard_handler();

IRQ_HANDLER(keyboard_handler){
    unsigned char status;
	char keycode;

	/* write EOI */
	outport(0x20, 0x20);

	status = inport(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inport(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		tty_out_char(keyboard_map[(uint16_t)keycode]);
	}
}



void keyboard_init()
{
	/* Initial handers of interrupts */
	idt_handler(0x21, keyboard_handler, 0x8E);
}
