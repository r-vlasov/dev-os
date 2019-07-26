#ifndef HUI23
#define HUI23
#include "lib/headers/stdlib.h"

#define KHEAP_START		0xc0000000
#define KHEAP_START_SIZE	0x100000;


#define ALLOC_PTR() 	2
#define SIZEOF(a)		(sizeof(a))
#define NEXT_CHUNK_PTR(p,s)	((heap_chunk_t*)(((size_t)p) + (sizeof(heap_chunk_t)) + s))



/* Memory allocation like "placement new" */
extern placement_address;



typedef struct MemChunk {
	struct 		MemChunk* next;
	struct 		MemChunk* prev;
	uint8_t	 	allocated; // 0 - no allocated,	1 - allocated
	uint32_t 	size;
} heap_chunk_t;


typedef struct kerHeap  {
	uint32_t 	start_addr;
	uint32_t 	end_addr;
	uint32_t 	max_addr;
	uint8_t  	readonly;
	heap_chunk_t*	head;
} heap_t;


heap_t *heap;

void create_heap(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly)
{
	heap = (heap_t*) start;
	heap->start_addr = start;
	heap->end_addr = start + size;
	heap->max_addr = start + max;
	heap->readonly = readonly;
	heap->head = (heap_chunk_t*)start;
	heap->head->prev = NULL;
	heap->head->next = NULL;
	heap->head->size = size - SIZEOF(heap_chunk_t);
}

static heap_chunk_t* find_chunk(uint32_t size, heap_t* heap)
{
	heap_chunk_t* result = NULL;
	heap_t* tmp = NULL;
	for (heap_chunk_t* chunk = heap->head; chunk != NULL && result == NULL; chunk = chunk->next)
	{
		if((chunk->size > size + SIZEOF(heap_chunk_t) + 1) && !chunk->allocated)
			result = chunk;
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

void* malloc1(uint32_t size)
{
	heap_chunk_t* result = find_chunk(size, heap);
	insert_chunk(result, size, heap);
	return (void*)(((uint32_t)result) + SIZEOF(heap_chunk_t));
}

#endif

