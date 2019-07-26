#include "../../../lib/headers/stdlib.h"
#include "../headers/tty.h"
#define	VIDEOMEMORY	0xB8000
#define	MOVECURSOR_STR	1

TTY tty;

void tty_init() 		// Initializing teletype(i meant console :)
{	
	// Initializing tty
	tty.width = *((uint16_t*)0x44A);
	tty.height = 25;
	tty.io_port = *((uint16_t*)0x463);
	tty.cursor = (*((uint8_t*)0x451))*tty.width + (*((uint8_t*)0x450));
	tty.buffer = (void*)0xB8000;
	tty.textattr = 7;
}	


void tty_out_char(char ch)	// Print 'ch' in tty.cursor position
{
	switch (ch) {
		case '\n':
			tty_move_cursor((tty.cursor / tty.width + 1) * tty.width);
			break;
		case '\b':
			tty_move_cursor(tty.cursor-1);
			tty.buffer[tty.cursor].symbol_ch = ' ';
			tty.buffer[tty.cursor].symbol_attr = tty.textattr;
			break;
		case '\t':
			while( (++tty.cursor) % 8 )
			{};
			tty_move_cursor(tty.cursor);
			break;
		default:
			tty.buffer[tty.cursor].symbol_ch = ch;
			tty.buffer[tty.cursor].symbol_attr = tty.textattr;
			tty_move_cursor(tty.cursor + 1);
	}
}

void tty_move_cursor(uint32_t pos)	// Move the cursor 
{
	tty.cursor = pos;
	if (tty.cursor >= tty.width * tty.height) 
	{
		tty.cursor = (tty.height - 1) * tty.width;
		memcpy(tty.buffer, tty.buffer + tty.width, tty.width * tty.height * sizeof(TTY_unit));
		memset(tty.buffer + tty.width * (tty.height - 1), (tty.textattr << 8) + ' ', tty.width);
	}
	asm("movb $0x0E, %%al \n movb %%ch, %%ah \n outw %%ax, %%dx \n incb %%al \n movb %%cl, %%ah \n outw %%ax, %%dx"
		::"d"(tty.io_port),"c"(tty.cursor));
}
void tty_clear_screen()
{
	TTY_unit* ptr = (TTY_unit*)tty.buffer;
	for(int i = 0; i < tty.width * (tty.height - 1); i++)
	{
		ptr[tty.width + i].symbol_ch = ' ';
		ptr[tty.width + i].symbol_ch = 0x00;

	}	
}		

void tty_write_string(const char* str)
{
	for(; *str; str++)
	{
		tty_out_char(*str);
	}
}

void tty_write_address(const uint32_t address)
{
	int tmp;
	int mask = 1;
	for (int i = 28; i > 0; i -= 4)
	{
		tmp = ( address >> i ) & 0xF;
		if(tmp == 0 && mask == 1)
		{
			continue;
		}
		if (tmp >= 0xA)
			tty_out_char('A' + tmp - 0xA);
		else
			tty_out_char('0' + tmp);
		mask = 0;
	}
	tmp = address & 0xF;
	if (tmp >= 0xA)
		tty_out_char(tmp-0xA+'A');
	else
		tty_out_char(tmp+'0');
}


