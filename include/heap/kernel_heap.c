#include "../../lib/headers/stdlib.h"

extern uint32_t kernel_end;   				//  Define in link.ld
uint32_t placement_address = (uint32_t)&kernel_end  	//  Heap start address (after kernel)

