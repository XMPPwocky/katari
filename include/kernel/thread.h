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


/* asm */


/* enterthread() is a funny sort of function
 * it switches to another thread and acts as if that thread's enterthread()
 * or gothread() call has returned (eg, popping registers and bx lr). The 
 * return value is the thread ID of the thread that called it.
 * Think about it and read the code- it makes sense. */
extern tid_t enterthread(struct Thread *t);

/* Handles the dirty work of actually restoring registers. */
extern tid_t gothread(struct Thread *t);
