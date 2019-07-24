#include "../../lib/stdlib.h"

/* We need a structure where we will contain frame of pages */
/* It is bitmap */

#define INDEX(a)  	(a/32)
#define OFFSET(a) 	(a%32)
#define PAGE_SIZE 	0x1000


uint32_t* 	page_frames;
uint32_t 	nframes;


	void	set_frame_bitmap(int i);
	void 	unset_frame_bitmap(int i);
	int 	get_frame_bitmap(int i);
	void 	search_clear_bit();
