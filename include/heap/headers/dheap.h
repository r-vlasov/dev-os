#ifndef DHEAP_H
#define DHEAP_H


/*	Dynamic heap(dmalloc, dfree)
**  	Our allocator uses linked lists in his structure
**  	Struct MemChunk describes entries of this Link.Lists
**	Struct KerHeap describes heap in general :((
** 	In my kernel might be a several heaps with flag "readonly" 
*/

#include "../../../lib/headers/stdlib.h"

#define KHEAP_START		0xc0000000
#define KHEAP_START_SIZE	0x100000
#define KHEAP_MAX		0x500000
#define KHEAP_PIECE		0x100000
#define KHEAP_SIZE(heap)	(heap->end_addr - heap->start_addr)


#define GET_CHUNK_PTR(ptr)		((heap_chunk_t*)(((uint32_t)ptr) - (sizeof(heap_chunk_t))))
#define ALLOC_DATA_PTR(res) 		(void*)(((uint32_t)res) + SIZEOF(heap_chunk_t))
#define SIZEOF(a)			(sizeof(a))
#define NEXT_CHUNK_PTR(p,s)		((heap_chunk_t*)(((uint32_t)p) + (sizeof(heap_chunk_t)) + s))
#define INCREASE_SIZE(size, end, max)	(((max - end) > KHEAP_PIECE) ? (size + KHEAP_PIECE) : (size + max - end))


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

void heap_init();

static void create_heap(uint32_t start, uint32_t size, uint32_t max, uint8_t readonly);
static heap_chunk_t* find_chunk(uint32_t size, heap_t* heap);
heap_chunk_t* find_chunk_align(uint32_t size, heap_t* heap);
heap_chunk_t* insert_chunk_align(heap_chunk_t* chunk, uint32_t size, heap_t* heap);
static void insert_chunk(heap_chunk_t* chunk, uint32_t size, heap_t* heap);
uint8_t extend(uint32_t new_size, heap_t *heap);
uint32_t alloc(uint32_t sz, uint8_t page_align, heap_t* heap);


// Allocation //	

void* dmalloc(uint32_t size);
void* dmalloc_align(uint32_t size);


// Freeing
void dfree(void* pointer);

#endif
