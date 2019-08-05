#ifndef PROCESS_STACK_H
#define PROCESS_STACK_H

#include "../../../lib/headers/stdlib.h"

void move_stack(void *new_stack_start, uint32_t size);
void copy_stack(void *new_stack_start, uint32_t size);

#endif
