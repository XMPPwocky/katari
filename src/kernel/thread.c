#include	"kernel/kernel.h"
#include	"kernel/thread.h"
#include	"kernel/panic.h"
#include	"kernel/message.h"

semaphore_t thread_table_lock = 0;
struct Thread *thread_table[MAX_THREADS];

void threadtable_init(void) {
	size_t i;
	for (i = 0; i < MAX_THREADS; i++) {
		thread_table[i] = NULL;
	};

	/* Make thread table usable */
	semaphore_V(thread_table_lock);
	
	return;
};

struct Thread *create_thread(register_t lr, register_t cpsr,
		struct AddressSpace *addrspace) {
	semaphore_P(thread_table_lock);
	
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
	thread->state->retstate[0] = lr;
	thread->state->retstate[1] = cpsr;
	unsigned char i;
	for (i = 0; i < 15; i++) {
		thread->state->registers[i] = 0xDEADBEEF;
	};	

	thread->id = id;
	thread->lock = 0;
	thread->status = THREAD_STATUS_INITIALIZING;

	thread->addrspace = addrspace;
	
	thread->block_type = THREAD_BLOCKED_NOTBLOCKED;
	BITMAP_ZERO(thread->block_mask, MAX_TID);

	BITMAP_ZERO(thread->pending_notifies, MAX_TID);


	semaphore_V(thread->lock);
	semaphore_V(thread_table_lock);

	return thread;
};

enum exception enter_thread(struct Thread *thread) {
	semaphore_P(thread->lock);

	thread->status = THREAD_STATUS_RUNNING;
	register_t retval = _enter_thread((register_t)thread);

	semaphore_V(thread->lock);

	return (enum exception)retval;
};
