#pragma once

#include "kernel/kernel.h"

#define SC_CALL_NOP 0

#define SC_RESULT_SUCCESS 0 /* success. duh. */
#define SC_RESULT_NOSUCHCALL 1 /* invalid syscall ID */
#define SC_RESULT_NOSUCHTHREAD 2 /* no such thread. duh. */
#define SC_RESULT_NOSUCHCONTEXT 3 /* you can figure this out */
#define SC_RESULT_TEMPFAIL 4 /* think EAGAIN. */
#define SC_RESULT_IHATEYOU 5 /* something's terribly wrong */
#define SC_RESULT_OOM 6 /* Not enough memory to complete requested action */

#define SYSCALL_RETURNANDSWITCH(thread, status, to) \
	thread->state->registers[0] = status; \
	semaphore_V(thread->lock); \
	return to
#define SYSCALL_RETURN(thread, status) \
	SYSCALL_RETURNANDSWITCH(thread, status, thread)

extern struct Thread *do_syscall(struct Thread *thread);
