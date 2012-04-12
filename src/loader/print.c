#include "stddef.h"

/* what's that? this is a total hack, you say? why yes, yes it is. */
volatile int *uart0 = (int *)0x44E09000;
volatile int *uart0_lsr = (int *)0x44E09014;

void putc(char c) {
	while (1) {
		int x = *uart0_lsr;
		x = x >> 5;
		if (x & 1) {
			break;
		};
	};
	*uart0 = c;
};

void print(char msg[]) {
	size_t i;
	for (i = 0; msg[i] != 0; i++) {
		putc(msg[i]);
	};
};

