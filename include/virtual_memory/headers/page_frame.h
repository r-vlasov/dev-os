#ifndef PAGE_FRAME
#define PAGE_FRAME
#include "../../../lib/headers/stdlib.h"
#include "paging.h"



#define INDEX(a)  	(a/32)
#define OFFSET(a) 	(a%32)
#define PAGE_SIZE 	0x1000


/////////////////////////////////
#define __ERROR_FRAME_CODE	-111
#define __NOFRAME		0

/* We need a structure where we will contain frame of pages */
/* It will be bitmap */

uint32_t* 	page_frames;
uint32_t 	nframes; 

/*	 Bitmap methods	 */
	void	set_frame_bitmap(uint32_t i);
	void 	unset_frame_bitmap(uint32_t i);
	int 	get_frame_bitmap(uint32_t i);
	int 	search_clear_bit();

	
	
/*	Allocation frames	*/
	void alloc_frame(page_t*, uint8_t, uint8_t);
	void free_frame(page_t*);

#endif
