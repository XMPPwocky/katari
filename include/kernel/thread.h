#pragma once

#include	"kernel/kernel.h"
#include	"kernel/exception.h"
#include	"kernel/thread.h"

typedef BITMAP(MAX_TID) threadmask_t;

enum ThreadStatus {
	THREAD_STATUS_INITIALIZING,
	THREAD_STATUS_BLOCKED,
	THREAD_STATUS_RUNNABLE,
	THREAD_STATUS_RUNNING
};

struct ThreadState {
	register_t retstate[2]; /* LR and CPSR */
	register_t registers[15]; /* r0-r14 */
};

struct Thread {
	struct ThreadState *state;
	
	tid_t id;
	semaphore_t lock;
	enum ThreadStatus status;

	uint8_t asid; /* zero if unassigned */
	void *translation_tables; /* physical address! */

	bool isblocked;
	tid_t blocked_on;
	threadmask_t recvmask;
	threadmask_t whackmask;
};

extern semaphore_t thread_table_lock;
extern struct Thread *thread_table[MAX_THREADS];

extern void threadtable_init(void);
extern struct Thread *create_thread(register_t lr, register_t cpsr, 
		void *translation_tables);

extern enum exception enter_thread(struct Thread *t);
extern register_t _enter_thread(register_t thread);
