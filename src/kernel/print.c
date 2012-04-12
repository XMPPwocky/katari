#include "kernel/kernel.h"

void print(char *msg) {
	for (; *msg != 0; msg++) {
		kputc(*msg);
	};
};

#ifdef SOC_AM335X
volatile uint32_t *uart0_thr = (uint32_t *)0x44E09000;
volatile uint32_t *uart0_lsr = (uint32_t *)0x44E09014;

void kputc(char c) {
	/* Wait for the THR to be empty */
	while (!(*uart0_lsr & MASK_BITAT(5)));

	*uart0_thr = c;
};
#endif
