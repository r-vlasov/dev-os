#include "../headers/dheap.h"
#include "../../memory_management/headers/paging.h"
#include "../../memory_management/headers/page_frame.h"

extern	page_directory_t* kernel_directory;
extern 	heap_t *heap;
extern 	page_t* get_page(uint32_t address, int make, page_directory_t *dir);
extern 	void alloc_frame(page_t*, uint8_t, uint8_t);


void heap_init()
{
	create_heap(KHEAP_START, KHEAP_START_SIZE, KHEAP_MAX, 0);
}

static void create_heap(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly)
{
	heap = (heap_t*) start;
	start += SIZEOF(heap_t);
	heap->start_addr = start;
	heap->end_addr = start + size;
	heap->max_addr = start + max;
	heap->readonly = readonly;
	heap->head = (heap_chunk_t*)(start);
	heap->head->prev = NULL;
	heap->head->next = NULL;
	heap->head->size = size - SIZEOF(heap_chunk_t);
}

static heap_chunk_t* find_chunk(uint32_t size, heap_t* heap)
{
	heap_chunk_t* result = NULL;

	for (heap_chunk_t* chunk = heap->head; chunk != NULL && result == NULL; chunk = chunk->next)
	{
		if(chunk->size >= size && !chunk->allocated)
			result = chunk;
	}
	return result;
}

static heap_chunk_t* find_chunk_align(uint32_t size, heap_t* heap)
{
	heap_chunk_t* result = NULL;
	uint32_t new_size;
	uint32_t offset;
	for (heap_chunk_t* chunk = heap->head; chunk != NULL && result == NULL; chunk = chunk->next)
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


static void insert_chunk(heap_chunk_t* chunk, uint32_t size, heap_t* heap)
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



static heap_chunk_t* insert_chunk_align(heap_chunk_t* chunk, uint32_t size, heap_t* heap)
{
	uint32_t offset;
	offset = PAGE_SIZE - ( (uint32_t)chunk + 2 * SIZEOF(heap_chunk_t) ) % PAGE_SIZE;
	insert_chunk(chunk, offset, heap);
	chunk->allocated = 0;
	insert_chunk(chunk->next, size, heap);
	return chunk->next;
}


static uint8_t extend(uint32_t new_size, heap_t *heap)
{
	if (new_size > heap->max_addr - heap->start_addr)
		return 0;

	new_size = new_size & 0xFFFFF000;
	new_size += PAGE_SIZE;
	
	uint32_t old_size = heap->end_addr - heap->start_addr;

	for (uint32_t i = old_size; i < new_size; i+= PAGE_SIZE)
	{
		alloc_frame( get_page(heap->start_addr + i, 1, kernel_directory), 0, 0);
	}
	
	//* KHEAP_START->obj1->obj2->obj3 so we should increse obj3 size, if it's not allocated *//
	heap_chunk_t* chunk;
	for (chunk = heap->head; chunk->next != NULL; chunk = chunk->next);
	tty_write_address(chunk);
	tty_out_char('\n');
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
	
	heap->end_addr = heap->start_addr + new_size;
	return 1;
}



void* dmalloc(uint32_t size)
{	
	heap_chunk_t* result = find_chunk(size, heap);	
	while (result == NULL)
	{
		if (HEAP_GROW(KHEAP_SIZE(heap)))
		{
			result = find_chunk(size, heap);
		}
		else
		{
			return NULL;
		}
	}	
	insert_chunk(result, size, heap);
	return ALLOC_DATA_PTR(result);
}



void* dmalloc_align(uint32_t size)
{
	heap_chunk_t* result = find_chunk_align(size, heap);
	while (result == NULL)
	{
		if (HEAP_GROW(KHEAP_SIZE(heap)))
		{
			result = find_chunk_align(size, heap);
		}
		else
		{
			return NULL;
		}
	}	
	result = insert_chunk_align(result, size, heap);
	return ALLOC_DATA_PTR(result);
}


void dfree(void* pointer)
{
	heap_chunk_t* chunk = GET_CHUNK_PTR(pointer);
	chunk->allocated = 0;

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


