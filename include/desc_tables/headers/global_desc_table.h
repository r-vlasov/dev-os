#include "../../../lib/headers/stdlib.h"

typedef struct {
	uint16_t limit_lowerbits;
	uint16_t base_lowerbits;
	uint8_t base_middlebits;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_higherbits;
} __attribute__((packed)) GDT_Node;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) GDTR;

void gdt_init();
void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

