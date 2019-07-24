#ifndef TTY_H
#define TTY_H
#include "../../../lib/headers/stdlib.h"

/* VideoMemory has of 2000words (for 80x25)
 * The first byte of word 	-	symbol's code
 * The second byte of word 	-	attribute(colours)
 */

/* The structure which describes videomemory unit */
typedef struct {
	uint8_t symbol_ch;
	uint8_t symbol_attr;
} __attribute__((packed)) TTY_unit;

/* TTY structure */
typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t cursor;
	TTY_unit* buffer;
	uint16_t io_port;
	uint8_t  textattr;
} TTY;



void tty_init(); 		// Initializing teletype(i meant console :)

void tty_out_char(char);
void tty_move_cursor(uint32_t);	// move the cursor 
void tty_clear_screen();		

void tty_write_string(const char*);
void tty_write_address(const uint32_t);
#endif
