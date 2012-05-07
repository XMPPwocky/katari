#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/thread.h"
#include "kernel/util.h"

void init(void) {
	semaphore_V(global_translation_tables_lock);

	exception_init();
	
	threadtable_init();
};
