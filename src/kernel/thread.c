#include "kernel/kernel.h"
#include "kernel/thread.h"

struct Thread *thread_table[MAX_THREADS];

struct Thread *curr_thread = NULL;

void threadtable_init(void) {
	tid_t i;
	for (i = 0; i < MAX_THREADS; i++) {
		thread_table[i] = NULL;
	};
};
