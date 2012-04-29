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
	cid_t context;
	semaphore_t lock;
	enum ThreadStatus status;

	void *mbb;
	tid_t blocked_on;
	threadmask_t recvmask;
};

extern semaphore_t thread_table_lock;
extern struct Thread *thread_table[MAX_THREADS];

extern void threadtable_init(void);
extern struct Thread *create_thread(register_t lr, register_t cpsr);

extern enum exception enter_thread(struct Thread *t);
extern register_t _enter_thread(struct Thread *t);
