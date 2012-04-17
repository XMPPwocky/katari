#pragma once
typedef unsigned int tid_t;

#define STACK_PER_THREAD 8192
#define MAX_THREADS	65536 /* this should probably be bigger */

struct Thread {
	char *sp;
	tid_t id;

	char stack[STACK_PER_THREAD]; /* This is kernel stack (per-thread) */
};

extern struct Thread *threadtable[MAX_THREADS];

extern void threadtable_init(void);
