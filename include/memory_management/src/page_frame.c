#include "../headers/page_frame.h"
#include "../../../drv/teletype/headers/tty.h"


void	set_frame_bitmap(uint32_t i)
{
	page_frames[INDEX(i)] |= 1 << OFFSET(i);
}

void 	unset_frame_bitmap(uint32_t i)
{
	page_frames[INDEX(i)] &= ~(1 << OFFSET(i));
}

int 	get_frame_bitmap(uint32_t i)
{
	return page_frames[INDEX(i)] & (1 << OFFSET(i));
}

int 	search_clear_bit() 
{
	uint32_t mask;
	for (uint32_t i = 0; i < INDEX(nframes); i++)
	{
		for (int j = 0; j < 32; j++)
		{
			mask = 0x1 << j;
			if (!(page_frames[i] & mask))
				return i*32+j;
		}
	}
	return -1;
}



void alloc_frame(page_t *page, uint8_t kernel, uint8_t writeable)
{
	 if(page->frame != 0)
		 return;
	 else
	 {
		 int32_t clrindex = search_clear_bit();
		 if (clrindex == -1 )
		 {
			tty_write_string("Haven't free frame");
			return;
		 }
		 set_frame_bitmap(clrindex);
		 page->present = 1;
		 page->rw = !writeable;
		 page->user = kernel;
		 page->frame = clrindex;
	 }
}


void free_frame(page_t *page)
{
	uint32_t frame = page->frame;
	if (!frame)
		return;
	else
	{
		unset_frame_bitmap(frame);
		page->frame = __NOFRAME;
	}
}

