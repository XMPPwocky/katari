#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/thread.h"
#include "kernel/util.h"

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

	print("Making new threads...\r\n");
	struct Thread *thread1 = create_thread((register_t)&dostuff, 0x1F);
	thread1->state->registers[13] = (int)((char *)kmalloc(8192) + 8192);
	thread1->state->registers[13] -= (thread1->state->registers[13] % 8);
	thread1->state->registers[0] = 0;
	struct Thread *thread2 = create_thread((register_t)&dostuff, 0x1F);
	thread2->state->registers[13] = (int)((char *)kmalloc(8192) + 8192);
	thread2->state->registers[13] -= (thread2->state->registers[13] % 8);
	thread2->state->registers[0] = 1;
	int i;
	while (true) {
		print("Entering thread1...\r\n");
		i = enter_thread(thread1);
		if (i == EXCEPTION_SVC) {
			print("thread1 did SVC\r\n");
		} else {
			print("thread1 did something else\r\n");
		};

		print("Entering thread2...\r\n");
		i = enter_thread(thread2);
		if (i == EXCEPTION_SVC) {
			print("thread2 did SVC\r\n");
		} else {
			print("thread2 did something else\r\n");
		};
	};
	print("Bye!\r\n");
};
