#pragma once

#include	"kernel/kernel.h"

typedef int tid_t;

#define MAX_THREADS 65536

struct ThreadState {
	int retstate[2]; /* LR and CPSR */
	int registers[15]; /* r0-r14 */
};

struct Thread {
	struct ThreadState *state;
	
	tid_t id;
};

extern struct Thread *thread_table[MAX_THREADS];
