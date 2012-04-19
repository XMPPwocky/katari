#pragma once

#include	"kernel/kernel.h"

typedef int tid_t;

#define MAX_THREADS 65536

struct ThreadState {
	register_t retstate[2]; /* LR and CPSR */
	register_t registers[15]; /* r0-r14 */
};

struct Thread {
	struct ThreadState *state;
	
	tid_t id;
};

extern struct Thread *thread_table[MAX_THREADS];

extern void threadtable_init(void);
extern tid_t create_thread(register_t lr, register_t cpsr, register_t r0);

extern unsigned int enter_thread(struct Thread *t);
