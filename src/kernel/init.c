#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/thread.h"
#include "kernel/util.h"

void init(void) {
	exception_init();
	threadtable_init();
};
