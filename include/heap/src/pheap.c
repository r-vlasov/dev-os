#include "../headers/pheap.h"

extern uint32_t kernel_end;   				//  Define in link.ld
extern uint32_t placement_address = (uint32_t)&kernel_end; 	//  Heap start address (after kernel)

uint32_t pmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (align && (placement_address & 0xFFFFF000) )
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t pmalloc_a(uint32_t sz)
{
    return pmalloc_int(sz, 1, 0);
}

uint32_t pmalloc_p(uint32_t sz, uint32_t *phys)
{
    return pmalloc_int(sz, 0, phys);
}

uint32_t pmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return pmalloc_int(sz, 1, phys);
}

uint32_t pmalloc(uint32_t sz)
{
    return pmalloc_int(sz, 0, 0);
}
