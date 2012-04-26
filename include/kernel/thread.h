#pragma once

#include	"kernel/kernel.h"
#include	"kernel/exception.h"

struct ThreadState {
	register_t retstate[2]; /* LR and CPSR */
	register_t registers[15]; /* r0-r14 */
};

struct Thread {
	struct ThreadState *state;
	
	tid_t id;
	semaphore_t lock;
};

extern semaphore_t thread_table_lock;
extern struct Thread *thread_table[MAX_THREADS];

extern void threadtable_init(void);
extern struct Thread *create_thread(register_t lr, register_t cpsr);

extern enum exception enter_thread(struct Thread *t);
extern register_t _enter_thread(struct Thread *t);
