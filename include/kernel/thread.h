#pragma once

struct ThreadState {
	int retstate[2]; /* LR and CPSR */
	int registers[15]; /* r0-r14 */
};
