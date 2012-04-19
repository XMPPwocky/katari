#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/util.h"

/* Linkerscript-defined symbols
 * Their values HAVE NO MEANING.
 * The data is in their address. Kinda hackish. */
extern int __heap_start_addr;
extern int __heap_size;

void *heap_start_addr = &__heap_start_addr;
size_t heap_size = (size_t)&__heap_size;

void init(void) {
	print("Katari starting up...\r\n");
	
	print("Setting up exception handling...\r\n");
	exception_init();
	
	print("Initializing heap...\r\n");
	heap_init(heap_start_addr, heap_size);

	print("Printing numbers...\r\n");
	char buf[32];
	inttostr(-1, buf, 32);
	print(buf);
	print("\r\n");
};
