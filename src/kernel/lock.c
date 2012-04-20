#include "kernel/kernel.h"

enum semaphore_op_result semaphore_V(semaphore_t sem) {
	return SEMAPHORE_FAIL_OTHER;
};

enum semaphore_op_result semaphore_T(semaphore_t sem) {
	return SEMAPHORE_FAIL_OTHER;
};
