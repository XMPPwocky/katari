#pragma once

#include "kernel/kernel.h"

#define SC_TYPE_NOP 0

#define SC_RESULT_SUCCESS 0
#define SC_RESULT_INVALIDSC 1
#define SC_RESULT_INVALIDPORT 2
#define SC_RESULT_NACHOS 3

#define SYSCALL_RETURNANDSWITCH(thread, status, to) \
	thread->state->registers[0] = status; \
	semaphore_V(thread->lock); \
	return to
#define SYSCALL_RETURN(thread, status) \
	SYSCALL_RETURNANDSWITCH(thread, status, thread)

extern struct Thread *do_syscall(struct Thread *thread);
