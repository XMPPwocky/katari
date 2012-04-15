#pragma once
typedef unsigned int tid_t;

#define KERNEL_THREADSTACK_SIZE 8192

struct Thread {
	void *sp;
	char kernel_stack[KERNEL_THREADSTACK_SIZE];
};
