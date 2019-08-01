#include "../headers/dheap.h"
#include "../../memory_management/headers/paging.h"
#include "../../memory_management/headers/page_frame.h"

extern	page_directory_t* kernel_directory;
extern 	heap_t *heap0;
extern 	page_t* get_page(uint32_t address, int make, page_directory_t *dir);
extern 	void alloc_frame(page_t*, uint8_t, uint8_t);


void heap_init(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly, heap_t** theap)
{
	create_heap(start, size, max, readonly, theap);
}

static void create_heap(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly, heap_t** theap)
{
	(*theap) = (heap_t*) start;
	start += SIZEOF(heap_t);
	(*theap)->start_addr = start;
	(*theap)->end_addr = start + size;
	(*theap)->max_addr = start + max;
	(*theap)->readonly = readonly;
	(*theap)->head = (heap_chunk_t*)(start);
	(*theap)->head->prev = NULL;
	(*theap)->head->next = NULL;
	(*theap)->head->size = size - SIZEOF(heap_chunk_t);
}

static heap_chunk_t* find_chunk(uint32_t size, heap_t* theap)
{
	heap_chunk_t* result = NULL;

	for (heap_chunk_t* chunk = theap->head; chunk != NULL && result == NULL; chunk = chunk->next)
	{
		if(chunk->size >= size && !chunk->allocated)
			result = chunk;
	}
	return result;
}

static heap_chunk_t* find_chunk_align(uint32_t size, heap_t* theap)
{
	heap_chunk_t* result = NULL;
	uint32_t new_size;
	uint32_t offset;
	for (heap_chunk_t* chunk = theap->head; chunk != NULL && result == NULL; chunk = chunk->next)
	{
		if ( chunk->size >= size )
		{
			if ( ((uint32_t)chunk + SIZEOF(heap_chunk_t)) & 0xFFFFF000 != 0 )
			{
				offset = PAGE_SIZE - ( (uint32_t)chunk + SIZEOF(heap_chunk_t) ) % PAGE_SIZE;
				new_size = chunk->size - offset; 
				if (new_size >= size)
					result = chunk;
			}
			else
			{
				result = chunk;
			}
		}
	}
	return result;
}


static void insert_chunk(heap_chunk_t* chunk, uint32_t size, heap_t* theap)
{
	heap_chunk_t* temp = NEXT_CHUNK_PTR(chunk, size);
	temp->allocated = 0;
	temp->size = chunk->size - size - SIZEOF(heap_chunk_t);
	temp->prev = chunk;
	temp->next = chunk->next;

	if(temp->next != NULL)
	{
		temp->next->prev = temp;
	}

	chunk->size = size;
	chunk->next = temp;
	chunk->allocated = 1;
}



static heap_chunk_t* insert_chunk_align(heap_chunk_t* chunk, uint32_t size, heap_t* theap)
{
	uint32_t offset;
	offset = PAGE_SIZE - ( (uint32_t)chunk + 2 * SIZEOF(heap_chunk_t) ) % PAGE_SIZE;
	insert_chunk(chunk, offset, theap);
	chunk->allocated = 0;
	insert_chunk(chunk->next, size, theap);
	return chunk->next;
}


static uint8_t extend(uint32_t new_size, heap_t *theap)
{
	if (new_size > theap->max_addr - theap->start_addr)
		return 0;

	new_size = new_size & 0xFFFFF000;
	new_size += PAGE_SIZE;
	
	uint32_t old_size = KHEAP_SIZE(theap);

	for (uint32_t i = old_size; i < new_size; i+= PAGE_SIZE)
	{
		alloc_frame( get_page(theap->start_addr + i, 1, kernel_directory), 0, 0);
	}
	
	//* KHEAP_START->obj1->obj2->obj3 so we should increse obj3 size, if it's not allocated *//
	heap_chunk_t* chunk;
	for (chunk = theap->head; chunk->next != NULL; chunk = chunk->next);
	if ( chunk->allocated )
	{
		heap_chunk_t* forw = NEXT_CHUNK_PTR(chunk, chunk->size);
		forw->next = NULL;
		forw->prev = chunk;
		forw->size = new_size - old_size - SIZEOF(heap_chunk_t);
		forw->allocated = 0;
	}
	else
	{
		chunk->size += new_size - old_size;
	}
//* *//	
	
	theap->end_addr = theap->start_addr + new_size;
	return 1;
}



void* dmalloc(uint32_t size, heap_t* theap)
{	
	heap_chunk_t* result = find_chunk(size, theap);	
	while (result == NULL)
	{
		if (HEAP_GROW(KHEAP_SIZE(theap), theap))
		{
			result = find_chunk(size, theap);
		}
		else
		{
			return NULL;
		}
	}	
	insert_chunk(result, size, theap);
	return ALLOC_DATA_PTR(result);
}



void* dmalloc_align(uint32_t size, heap_t* theap)
{
	heap_chunk_t* result = find_chunk_align(size, theap);
	while (result == NULL)
	{
		if (HEAP_GROW(KHEAP_SIZE(theap), theap))
		{
			result = find_chunk_align(size, theap);
		}
		else
		{
			return NULL;
		}
	}	
	result = insert_chunk_align(result, size, theap);
	return ALLOC_DATA_PTR(result);
}


void dfree(void* pointer, heap_t* theap)
{
	heap_chunk_t* chunk = GET_CHUNK_PTR(pointer);
	chunk->allocated = 0;
	
	if (chunk < theap->start_addr || chunk >= theap->end_addr)
	{
		tty_write_string("Can't freeing");
		return;
	}

	if(chunk->prev != NULL && !chunk->prev->allocated)
	{
		chunk->prev->next = chunk->next;
		chunk->prev->size += chunk->size + SIZEOF(heap_chunk_t);
		if (chunk->next != 0)
			chunk->next->prev = chunk->prev;
		chunk = chunk->prev;
	}

	if(chunk->next != NULL && !chunk->next->allocated)
	{
		chunk->next = chunk->next->next;
		chunk->size += chunk->next->size + SIZEOF(heap_chunk_t);
		if (chunk->next != 0)
			chunk->next->prev = chunk;
	}
}


