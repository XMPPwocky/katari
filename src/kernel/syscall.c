#include "kernel/kernel.h"
#include "kernel/syscall.h"
#include "kernel/thread.h"
#include "kernel/util.h"
#include "kernel/vm.h"

static inline struct Thread *syscall_nop(struct Thread *thread) {
	SYSCALL_RETURN(thread, SC_RESULT_SUCCESS);
};

static inline struct Thread *syscall_addthread(struct Thread *thread) {
	/* The pointer to the thread struct passed in here MUST be global. */

	if (!(thread->privileged)) {
		SYSCALL_RETURN(thread, SC_RESULT_PRIVILEGE);
	};

	struct Thread *newthread = (void *)thread->state->registers[0];

	if (!va_is_global(newthread)) {
		SYSCALL_RETURN(thread, SC_RESULT_BADDRESS);
	};

	thread->state->registers[1] = add_to_thread_table(newthread);

	SYSCALL_RETURN(thread, SC_RESULT_SUCCESS);
};

static inline struct Thread *syscall_invalid(struct Thread *thread) {
	SYSCALL_RETURN(thread, SC_RESULT_NOSUCHCALL);
};

struct Thread *do_syscall(struct Thread *thread) {
	semaphore_P(thread->lock);

	switch (thread->state->registers[12]) {
		case SC_CALL_NOP:
			return syscall_nop(thread);
			break;

		case SC_CALL_ADDTHREAD:
			return syscall_addthread(thread);
			break;
		default:
			return syscall_invalid(thread);
			break;
	};
};
