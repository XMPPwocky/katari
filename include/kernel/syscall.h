#pragma once

#include "kernel/kernel.h"

#define SC_CALL_NOP 0
#define SC_CALL_ALLOCPAGES 1 /* privileged */
#define SC_CALL_FREEPAGES 2  /* privileged */
#define SC_CALL_ADDTHREAD 3 /* privileged */
#define SC_CALL_DELTHREAD 4 /* privileged */
#define SC_CALL_SEND 5
#define SC_CALL_RECV 6
#define SC_CALL_CANSEND 7
#define SC_CALL_CANRECV 8

#define SC_RESULT_SUCCESS 0 /* success. duh. */
#define SC_RESULT_NOSUCHCALL 1 /* invalid syscall ID */
#define SC_RESULT_NOSUCHTHREAD 2 /* no such thread. duh. */
#define SC_RESULT_TEMPFAIL 4 /* think EAGAIN. */
#define SC_RESULT_IHATEYOU 5 /* something's terribly wrong */
#define SC_RESULT_OOM 6 /* Not enough memory to complete requested action */
#define SC_RESULT_PRIVILEGE 7 /* unprivileged thread tried to make 
			       privileged syscall */
#define SC_RESULT_BADDRESS 8 /* Bad address */

#define SYSCALL_RETURNANDSWITCH(thread, status, to) \
	thread->state->registers[0] = status; \
	semaphore_V(thread->lock); \
	return to
#define SYSCALL_RETURN(thread, status) \
	SYSCALL_RETURNANDSWITCH(thread, status, thread)

extern struct Thread *do_syscall(struct Thread *thread);
