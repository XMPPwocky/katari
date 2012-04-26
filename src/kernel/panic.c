#include "kernel/kernel.h"
#include "kernel/print.h"
#include "kernel/panic.h"

void panic(char *msg) {
	print(msg);

	BREAKPOINT;
};
