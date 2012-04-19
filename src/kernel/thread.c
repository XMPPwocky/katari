#include	"kernel/kernel.h"
#include	"kernel/thread.h"

struct Thread *thread_table[MAX_THREADS];

void threadtable_init(void) {
	size_t i;
	for (i = 0; i < MAX_THREADS; i++) {
		thread_table[i] = NULL;
	};
	
	return;
};

tid_t create_thread(register_t lr, register_t cpsr, register_t r0) {
	/* Find an available TID */
	tid_t id;
	bool found = false;
	for (id = 0; id < MAX_THREADS; id++) {
		if (thread_table[id] == NULL) {
			found = true;
			break;
		};
	};
	if (!found) {
		panic("Out of thread table slots!");
	};

	struct Thread *thread = kmalloc(sizeof (struct Thread));
	thread->state = kmalloc(sizeof (struct ThreadState));
	thread->id = id;

	thread->state->retstate[0] = lr;
	thread->state->retstate[1] = cpsr;

	thread->state->registers[0] = r0; /* initial r0 used for init., etc */

	int i;
	for (i = 1; i < 15; i++) { /* initialize the remaining 14 registers */
		thread->state->registers[i] = 0xAA55; /* credit to Laksen */
	};

	return thread->id;
};
