#include "../headers/dheap.h"


extern heap_t *heap;

void heap_init()
{
	create_heap(KHEAP_START, KHEAP_START_SIZE, KHEAP_MAX, 0);
}

void create_heap(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly)
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

void* dmalloc(uint32_t size)
{
	heap_chunk_t* result = find_chunk(size, heap);
	insert_chunk(result, size, heap);
	
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


