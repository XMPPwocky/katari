#include "kernel/kernel.h"

enum semaphore_op_result semaphore_try_V(semaphore_t sem) {
	bool result;
	semaphore_t foo = 0; /* used as a temporary variable to hold value */
	asm (\
			"ldrex	%[semval], [%[semaddr]]\n\t"
			"cmp	%[semval], #0xFFFFFFFF\n\t"
			"moveq	%[result], #1\n\t"
			"addne	%[semval], %[semval], #1\n\t"
			"strexne	%[result], %[semval], [%[semaddr]]\n\t"
			: [result]"=r" (result)
			: [semaddr]"r" (&sem), [semval]"r" (foo));
	if (result == 1) {
		return SEMAPHORE_FAIL_COLLISION;
	} else if (result == 2) {
		return SEMAPHORE_FAIL_VALUE;
	} else {
		return SEMAPHORE_SUCCESS;
	};
};

enum semaphore_op_result semaphore_try_T(semaphore_t sem) {
	return SEMAPHORE_FAIL_OTHER;
};

void semaphore_V(semaphore_t sem) {
	while (!(semaphore_try_V(sem) == SEMAPHORE_SUCCESS));
};

void semaphore_T(semaphore_t sem) {
	while (!(semaphore_try_T(sem) == SEMAPHORE_SUCCESS));
};
