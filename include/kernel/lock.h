#pragma once

#include "kernel/kernel.h"

typedef uint32_t semaphore_t;

enum semaphore_op_result {
	SEMAPHORE_SUCCESS,
	SEMAPHORE_FAIL_COLLISION, /* two simultaneous ops on same semaphore */
	SEMAPHORE_FAIL_VALUE, /* just not enough V ops */
	SEMAPHORE_FAIL_OTHER
};

void semaphore_V(semaphore_t sem);
void semaphore_T(semaphore_t sem);

enum semaphore_op_result semaphore_try_V(semaphore_t sem);
enum semaphore_op_result semaphore_try_P(semaphore_t sem);
