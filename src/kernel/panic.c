#include "kernel/kernel.h"
#include "kernel/panic.h"

void panic(char *msg) {
	print(msg);

	BREAKPOINT;
};
