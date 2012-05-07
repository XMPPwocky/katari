#include "kernel/kernel.h"
#include "kernel/exception.h"
#include "kernel/thread.h"
#include "kernel/util.h"
#include "kernel/print.h"

void init(void) {
	print("Katari starting up...\r\n");
	
	print("Setting up exception handling...\r\n");
	exception_init();
	
	print("Setting up thread table...\r\n");
	threadtable_init();

	print("Bye!\r\n");
};
