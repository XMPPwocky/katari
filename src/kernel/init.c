#include "kernel/kernel.h"

/* Linkerscript-defined symbols
 * Their values HAVE NO MEANING.
 * The data is in their address. Kinda hackish. */
extern int __heap_start_addr;
extern int __heap_size;

void *heap_start_addr = &__heap_start_addr;
size_t heap_size = (size_t)&__heap_size;
 
void init(void) {
	print("Katari starting up...\n");
	print("Initializing heap...\n");
	heap_init(heap_start_addr, heap_size);

	void *ptr = kalloc(1);
};
