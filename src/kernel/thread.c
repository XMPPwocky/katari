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

tid_t add_to_thread_table(struct Thread *thread) {
	semaphore_P(thread_table_lock);
	semaphore_P(thread->lock);

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

	thread->id = id;

	semaphore_V(thread->lock);
	semaphore_V(thread_table_lock);

	return id;
};
	
enum exception enter_thread(struct Thread *thread) {
	semaphore_P(thread->lock);

	thread->status = THREAD_STATUS_RUNNING;
	register_t retval = _enter_thread((register_t)thread);

	semaphore_V(thread->lock);

	return (enum exception)retval;
};
