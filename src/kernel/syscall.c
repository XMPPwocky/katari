#include "kernel/kernel.h"
#include "kernel/syscall.h"
#include "kernel/thread.h"
#include "kernel/print.h"
#include "kernel/util.h"

static inline struct Thread *syscall_nop(struct Thread *thread) {
	char tid_str[10];
	inttostr(thread->id, tid_str, 10);

	print("Thread ");
	print(tid_str);
	print(" did syscall_nop.\r\n");

	SYSCALL_RETURN(thread, SC_RESULT_SUCCESS);
};

static inline struct Thread *syscall_version(struct Thread *thread) {
	thread->state->registers[1] = KATARI_API_VERSION;

	SYSCALL_RETURN(thread, SC_RESULT_SUCCESS);
};

static inline struct Thread *syscall_invalid(struct Thread *thread) {
	char tid_str[10];
	inttostr(thread->id, tid_str, 10);

	print("Thread ");
	print(tid_str);
	print(" did an invalid syscall!\r\n");

	SYSCALL_RETURN(thread, SC_RESULT_INVALIDSC);
};

struct Thread *do_syscall(struct Thread *thread) {
	semaphore_P(thread->lock);

	switch (thread->state->registers[12]) {
		case SC_TYPE_NOP:
			return syscall_nop(thread);
			break;

		default:
			return syscall_invalid(thread);
			break;
	};
};
