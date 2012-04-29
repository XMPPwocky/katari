#pragma once

#include "kernel/kernel.h"

#define SYSTEM_CONTEXT 0

struct Context {
	cid_t id;

	unsigned int num_threads;
	tid_t threads[MAX_THREADS_PER_CONTEXT];

	bool use_virtual_memory;
	struct ContextVMConfig vm_config;
};

extern semaphore_t context_table_lock;
struct Context *context_table[MAX_CONTEXTS];
