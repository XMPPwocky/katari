#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/thread.h"
#include "kernel/util.h"
#include "kernel/print.h"

extern void dostuff(void);

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

	print("Setting up thread table...\r\n");
	threadtable_init();

	print("Bye!\r\n");
};
