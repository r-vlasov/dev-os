
#include "lib/headers/stdlib.h"


typedef struct {
	struct 		MemoryChunk *next;
	struct		MemoryChunk *prev;
	uint8_t 	allocated;
	uint32_t 	size;
} MemoryChunk;


MemoryChunk* first;

void create_heap(uint32_t start, uint32_t size)
{
	first = (MemoryChunk*)start;
	first->allocated = 0;
	first->prev = 0;
	first->next = 0;
	first->size = size - sizeof(MemoryChunk);
}

void* malloc1(size_t size)
{
    MemoryChunk *result = 0;
    
    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
	    if(chunk->size > size && !chunk->allocated)
		    result = chunk;
        
    if(result == 0)
        return 0;
    
    if(result->size >= size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        temp->allocated = 0;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
	{
		MemoryChunk* s = temp->next;
		s = s->prev;
            s = temp;
	}
        
        result->size = size;
        result->next = temp;
    }
    
    result->allocated = 1;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

#endif

