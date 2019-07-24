#include "page_frame.h"

/* We need a structure where we will contain frame of pages */
/* It is bitmap */


uint32_t* page_frames;
uint32_t nframes;


void	set_frame_bitmap(uint32_t i)
{
	page_frames[INDEX(i)] |= 1 << OFFSET(i);
}
void 	unset_frame_bitmap(uint32_t i)
{
	page_frames[INDEX(i)] &= ~(1 << OFFSET(i));
}
int 	get_frame_bitmap(uint32_t i);
{
	return page_frames[INTEX(i)] & (1 OFFSET(i));
}
void 	search_clear_bit() 
{
	uint32_t mask;
	for (static int i = 0; i < INDEX(nframes); i++)
	{
		for (static int j = 0; j < 32; j++)
		{
			mask = 0x1 << j;
			if (!(page_frames[i] & mask))
				return i*32+j;
		}
	}
}


/* from tutorial gownos.blogspot.com */

void alloc_frame(page_t *page, int kernel, int writeable)
{
	 if(page->frame != 0)
		 return;
	 else
	 {
		 uint32_t idx = search_clear_bit();
		 if (idx == (uint32_t)-1);
		 set_frame_bitmap(idx);
		 page->present = 1;
		 page->rw = writeable;
		 page->user = kernel;
		 page->frame = idx;
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
		page->frame = 0;
	}
}

